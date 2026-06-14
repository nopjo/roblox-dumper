#include "process.h"
#include "process/memory/memory.h"
#include <regex>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <elf.h>
#include <link.h>
#include <dlfcn.h>
#include <spdlog/spdlog.h>

namespace process {

    Process::~Process() {
        if (m_pid && m_attached) {
            ptrace(PTRACE_DETACH, m_pid, nullptr, 0);
            m_pid = 0;
            m_attached = false;
        }
    }

    auto Process::attach(std::string_view process_name) -> bool {
        if (m_attached && m_pid) {
            ptrace(PTRACE_DETACH, m_pid, nullptr, 0);
            m_pid = 0;
            m_attached = false;
            m_module_base = 0;
        }

        const auto pid = find_process_by_id(process_name);
        if (!pid) {
            return false;
        }

        m_pid = *pid;

        if (!attach_ptrace(m_pid)) {
            return false;
        }

        if (!cache_module_info()) {
            ptrace(PTRACE_DETACH, m_pid, nullptr, 0);
            m_pid = 0;
            return false;
        }

        m_attached = true;
        return true;
    }

    auto Process::attach_ptrace(pid_t pid) -> bool {
        if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) == -1) {
            spdlog::error("Failed to attach with ptrace: {}", strerror(errno));
            return false;
        }

        // Wait for the process to stop
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            spdlog::error("Failed to wait for process: {}", strerror(errno));
            ptrace(PTRACE_DETACH, pid, nullptr, 0);
            return false;
        }

        if (!WIFSTOPPED(status)) {
            spdlog::error("Process did not stop properly");
            ptrace(PTRACE_DETACH, pid, nullptr, 0);
            return false;
        }

        spdlog::info("Successfully attached to PID {} via ptrace", pid);
        return true;
    }

    auto Process::read_memory(uintptr_t address, void* buffer, size_t size) const -> bool {
        if (!m_attached || !m_pid) {
            return false;
        }

        unsigned char* data = reinterpret_cast<unsigned char*>(buffer);
        size_t bytes_read = 0;

        // ptrace can only read word-sized chunks at a time
        while (bytes_read < size) {
            long word = ptrace(PTRACE_PEEKDATA, m_pid, address + bytes_read, nullptr);
            if (word == -1 && errno != 0) {
                return bytes_read > 0;  // Return what we could read
            }

            size_t to_copy = std::min(sizeof(long), size - bytes_read);
            memcpy(data + bytes_read, &word, to_copy);
            bytes_read += to_copy;
        }

        return true;
    }

    auto Process::cache_module_info() -> bool {
        // Read /proc/[pid]/maps to find the main module base address
        std::string maps_path = "/proc/" + std::to_string(m_pid) + "/maps";
        std::ifstream maps_file(maps_path);

        if (!maps_file.is_open()) {
            spdlog::error("Failed to open {}", maps_path);
            return false;
        }

        std::string line;
        while (std::getline(maps_file, line)) {
            // Format: address perms offset dev inode pathname
            // Example: 56559c7c7000-56559c7d2000 r-xp 00000000 08:05 3669970 /path/to/binary
            std::istringstream iss(line);
            std::string addr_range, perms, offset, dev, inode, pathname;

            if (!(iss >> addr_range >> perms >> offset >> dev >> inode >> pathname)) {
                continue;
            }

            // We want the first executable mapping that's not a shared library
            if (perms[2] == 'x' && pathname[0] == '/' && pathname.find(".so") == std::string::npos) {
                // Parse the start address
                size_t dash_pos = addr_range.find('-');
                if (dash_pos != std::string::npos) {
                    m_module_base = std::stoull(addr_range.substr(0, dash_pos), nullptr, 16);
                    spdlog::info("Found module base at 0x{:x}", m_module_base);
                    return true;
                }
            }
        }

        return false;
    }

    auto Process::get_window_handle() const -> int {
        // X11 window detection would go here
        // For now, we'll return a placeholder
        return 0;
    }

    auto Process::get_window_dimensions() const -> std::optional<glm::vec2> {
        // X11 window dimensions would go here
        return std::nullopt;
    }

    auto Process::get_version() const -> std::optional<std::string> {
        if (!m_pid) {
            return std::nullopt;
        }

        // Read the command line to extract version info
        std::string cmdline_path = "/proc/" + std::to_string(m_pid) + "/cmdline";
        std::ifstream cmdline_file(cmdline_path);

        if (!cmdline_file.is_open()) {
            return std::nullopt;
        }

        std::string cmdline;
        std::getline(cmdline_file, cmdline);

        std::regex version_regex(R"(version-[a-f0-9]{16})");
        std::smatch match;

        if (std::regex_search(cmdline, match, version_regex)) {
            return match.str();
        }

        return std::nullopt;
    }

    auto Process::find_process_by_id(std::string_view process_name) -> std::optional<pid_t> {
        DIR* dir = opendir("/proc");
        if (!dir) {
            spdlog::error("Failed to open /proc");
            return std::nullopt;
        }

        std::optional<pid_t> result = std::nullopt;
        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr) {
            // Check if directory name is numeric (PID)
            if (entry->d_type != DT_DIR || !std::isdigit(entry->d_name[0])) {
                continue;
            }

            pid_t pid = std::stoi(entry->d_name);
            std::string cmdline_path = std::string("/proc/") + entry->d_name + "/cmdline";
            std::ifstream cmdline_file(cmdline_path);

            if (!cmdline_file.is_open()) {
                continue;
            }

            std::string cmdline;
            std::getline(cmdline_file, cmdline);

            // For Flatpak, look for 'sober' or the Roblox process
            // Flatpak processes have paths like /app/bin/sober or similar
            if (cmdline.find("sober") != std::string::npos || 
                cmdline.find(process_name) != std::string::npos ||
                cmdline.find("RobloxPlayerBeta") != std::string::npos) {
                spdlog::info("Found process {} with PID {}", process_name, pid);
                result = pid;
                break;
            }
        }

        closedir(dir);
        return result;
    }

} // namespace process
