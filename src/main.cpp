#include "control/control_server.hpp"
#include "memory/memory.h"
#include "scanner/scanner.hpp"
#include "utils/config.hpp"
#include "utils/file_utils.hpp"
#include "utils/logger.hpp"
#include "utils/offset_registry.hpp"
#include <regex>

int main() {
    if (!config::init()) {
        LOG_ERR("Failed to init config");
        std::cin.get();
        return 1;
    }

    memory = std::make_unique<Memory>("RobloxPlayerBeta.exe");

    if (!memory->process_handle) {
        LOG_ERR("Failed to attach to Roblox");
        std::cin.get();
        return 1;
    }

    LOG_SUCCESS("Attached to Roblox, PID: {}", memory->process_id);

    control::ControlServer server(8000);
    server.start();

    if (!server.is_running()) {
        LOG_ERR("Failed to start control server");
        std::cin.get();
        return 1;
    }

    if (!scanner::run_all_phases()) {
        LOG_ERR("Dumping failed");
        std::cin.get();
        return 1;
    }

    LOG_INFO("Dumping complete");

    std::string exe_path = memory->get_executable_path();
    if (!exe_path.empty()) {
        std::regex version_regex(R"(version-([a-f0-9]+))");
        std::smatch match;
        if (std::regex_search(exe_path, match, version_regex)) {
            std::string version = "version-" + match[1].str();
            offset_registry.set_roblox_version(version);
        }
    }

    std::string output_path = file_utils::get_exe_directory() + "\\offsets.hpp";
    offset_registry.write_to_file(output_path);

    LOG_SUCCESS("Dumping Complete");
    LOG_SUCCESS("Offsets saved to: {}", output_path);

    std::cin.get();
    return 0;
}