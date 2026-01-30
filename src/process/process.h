#pragma once
// clang-format off
#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <string_view>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <glm/glm.hpp>

// clang-format on

typedef CLIENT_ID* PCLIENT_ID;

namespace process {

    class NtDll {
      public:
        NtDll();

        template <typename T = uintptr_t> auto get_export(const std::string& function_name) -> T {
            if (m_cache.find(function_name) == m_cache.end()) {
                m_cache[function_name] =
                    reinterpret_cast<uintptr_t>(GetProcAddress(m_module, function_name.c_str()));
            }
            return reinterpret_cast<T>(m_cache[function_name]);
        }

      private:
        HMODULE m_module;
        std::unordered_map<std::string, uintptr_t> m_cache;
    };

    class Process {
      public:
        Process() = default;
        ~Process();

        auto attach(std::string_view process_name) -> bool;
        auto get_pid() const -> DWORD { return m_pid; }
        auto get_handle() const -> HANDLE { return m_handle; }
        auto get_module_base() const -> uintptr_t { return m_module_base; }
        auto get_section(std::string_view section_name) const
            -> std::optional<std::pair<uintptr_t, size_t>>;
        auto get_window_dimensions() const -> std::optional<glm::vec2>;

      public:
        NtDll m_ntdll;

      private:
        auto nt_open_process(DWORD pid) -> HANDLE;
        auto find_process_by_id(std::string_view process_name) -> std::optional<DWORD>;
        auto cache_module_info() -> bool;
        auto get_window_handle() const -> HWND;

      private:
        HANDLE m_handle{};
        DWORD m_pid{};
        uintptr_t m_module_base{};
        bool m_attached{};
    };

    inline Process g_process;
} // namespace process