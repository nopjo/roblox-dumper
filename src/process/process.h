#pragma once
// clang-format off
#include <string_view>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <glm/glm.hpp>
#include <sys/types.h>

// clang-format on

namespace process {

    class Process {
      public:
        Process() = default;
        ~Process();

        auto attach(std::string_view process_name) -> bool;
        auto get_pid() const -> pid_t { return m_pid; }
        auto get_module_base() const -> uintptr_t { return m_module_base; }
        auto get_section(std::string_view section_name) const
            -> std::optional<std::pair<uintptr_t, size_t>>;
        auto get_window_dimensions() const -> std::optional<glm::vec2>;
        auto get_version() const -> std::optional<std::string>;

      private:
        auto find_process_by_id(std::string_view process_name) -> std::optional<pid_t>;
        auto cache_module_info() -> bool;
        auto get_window_handle() const -> int;  // X11 window ID
        auto attach_ptrace(pid_t pid) -> bool;
        auto read_memory(uintptr_t address, void* buffer, size_t size) const -> bool;

      private:
        pid_t m_pid{};
        uintptr_t m_module_base{};
        bool m_attached{};
    };

    inline Process g_process;
} // namespace process
