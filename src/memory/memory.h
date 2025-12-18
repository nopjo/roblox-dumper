#pragma once
#define NOMINMAX
#include "future"
#include "memory/rtti.h"
#include "utils/logger.hpp"
#include "utils/structs.h"
#include <TlHelp32.h>
#include <Windows.h>
#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <winternl.h>

typedef CLIENT_ID* PCLIENT_ID;

struct NtDll {
    uintptr_t base_address;
    NtDll();

    template <typename T = uintptr_t> T get_exported_function(std::string function_name) {
        static std::unordered_map<std::string, uintptr_t> map;
        if (map.find(function_name) == map.end())
            map[function_name] =
                uintptr_t(GetProcAddress((HMODULE)base_address, function_name.c_str()));
        return reinterpret_cast<T>(map[function_name]);
    }
};

struct RobloxStringInfo {
    size_t pointer_offset;
    size_t length_offset;
};

inline NtDll ntdll;

class Memory {
  public:
    DWORD process_id;
    HWND window_handle;
    std::string process_name;
    std::string window_name;
    HANDLE process_handle;

  public:
    Memory(std::string _process_name, std::string _window_name = "");

    bool attach_process(std::string process_name);
    bool attach_window(std::string window_name);
    HWND get_window_handle();
    uintptr_t base_address();
    std::pair<int, int> get_window_dimensions();

    template <typename T = uintptr_t> T read(uintptr_t address) {
        using tNtReadVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
        static tNtReadVirtualMemory nt_read =
            ntdll.get_exported_function<tNtReadVirtualMemory>("NtReadVirtualMemory");

        T buffer{};
        SIZE_T bytes_read = 0;
        NTSTATUS status = nt_read(process_handle, reinterpret_cast<PVOID>(address), &buffer,
                                  sizeof(T), &bytes_read);
        if (!NT_SUCCESS(status) || bytes_read != sizeof(T))
            return T{};
        return buffer;
    }

    template <typename T>
    std::optional<size_t> find_value_offset(uintptr_t base_address, T expected_value,
                                            size_t max_offset = 0x1000, size_t alignment = 0x8,
                                            size_t start_offset = 0,
                                            const std::vector<size_t>& exclude_offsets = {}) {
        for (size_t offset = start_offset; offset < max_offset; offset += alignment) {
            if (std::find(exclude_offsets.begin(), exclude_offsets.end(), offset) !=
                exclude_offsets.end()) {
                continue;
            }
            T value = read<T>(base_address + offset);
            if (value == expected_value)
                return offset;
        }
        return std::nullopt;
    }

    template <typename T>
    std::optional<size_t> find_verified_offset(const std::vector<uintptr_t>& addresses,
                                               const std::vector<T>& expected_values,
                                               size_t max_offset = 0x1000, size_t alignment = 0x8) {
        if (addresses.size() != expected_values.size() || addresses.empty())
            return std::nullopt;

        std::vector<size_t> candidates;
        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            T value = read<T>(addresses[0] + offset);
            if (value == expected_values[0])
                candidates.push_back(offset);
        }

        for (size_t offset : candidates) {
            bool all_match = true;
            for (size_t i = 1; i < addresses.size(); i++) {
                T value = read<T>(addresses[i] + offset);
                if (value != expected_values[i]) {
                    all_match = false;
                    break;
                }
            }
            if (all_match)
                return offset;
        }
        return std::nullopt;
    }

    std::optional<size_t> find_verified_offset_float(const std::vector<uintptr_t>& addresses,
                                                     const std::vector<float>& expected_values,
                                                     size_t max_offset = 0x1000,
                                                     size_t alignment = 0x4,
                                                     float tolerance = 0.01f);

    template <typename T>
    std::vector<size_t> find_offsets_with_snapshots(uintptr_t base_address,
                                                    const std::vector<T>& known_values,
                                                    std::function<void(size_t)> value_changer,
                                                    size_t max_offset = 0x1000,
                                                    size_t alignment = 0x8, int sleep_ms = 500) {
        std::vector<size_t> candidates;
        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            T value = read<T>(base_address + offset);
            if (value == known_values[0])
                candidates.push_back(offset);
        }

        if (candidates.empty())
            return {};

        for (size_t i = 1; i < known_values.size(); i++) {
            value_changer(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

            std::vector<size_t> new_candidates;
            for (size_t offset : candidates) {
                T value = read<T>(base_address + offset);
                if (value == known_values[i])
                    new_candidates.push_back(offset);
            }

            candidates = std::move(new_candidates);
            if (candidates.empty()) {
                LOG_ERR("No candidates remaining after snapshot {}", i);
                return {};
            }
        }

        std::vector<size_t> verified;
        for (size_t offset : candidates) {
            T final_value = read<T>(base_address + offset);
            if (final_value == known_values.back())
                verified.push_back(offset);
        }

        return verified;
    }

    std::optional<size_t> find_vector3_offset(const std::vector<uintptr_t>& addresses,
                                              const std::vector<::Vector3>& expected_values,
                                              size_t search_range = 0x1000, float epsilon = 0.01f) {
        return find_verified_offset_custom<::Vector3>(
            addresses, expected_values,
            [epsilon](const ::Vector3& a, const ::Vector3& b) { return a.equals(b, epsilon); },
            search_range, 0x4);
    }

    template <typename T, typename Comparator>
    std::optional<size_t>
    find_verified_offset_custom(const std::vector<uintptr_t>& addresses,
                                const std::vector<T>& expected_values, Comparator compare,
                                size_t search_range = 0x1000, size_t alignment = 0x4) {
        if (addresses.size() != expected_values.size() || addresses.empty()) {
            return std::nullopt;
        }

        for (size_t offset = 0; offset < search_range; offset += alignment) {
            bool all_match = true;

            for (size_t i = 0; i < addresses.size(); i++) {
                T value = read<T>(addresses[i] + offset);

                if (!compare(value, expected_values[i])) {
                    all_match = false;
                    break;
                }
            }

            if (all_match) {
                return offset;
            }
        }

        return std::nullopt;
    }

    std::vector<uint8_t> read_bytes(uintptr_t address, size_t size);
    bool read_raw(uintptr_t address, void* buffer, size_t size);

    std::optional<std::pair<uintptr_t, size_t>> get_section_range(const char* section_name);

    std::string read_string(uintptr_t address, size_t max_length = 256);
    std::string read_roblox_string(uintptr_t address);

    std::optional<RttiInfo> scan_rtti(uintptr_t address);
    std::optional<size_t> find_rtti_offset(uintptr_t base_address, const std::string& target_class,
                                           size_t max_offset = 0x1000, size_t alignment = 0x8);
    std::optional<uintptr_t> find_pointer_by_rtti(const std::string& target_rtti,
                                                  size_t scan_size = 0x10000000,
                                                  size_t alignment = 0x8);
    std::vector<RttiInfo> scan_all_rtti(uintptr_t base_address, size_t max_offset = 0x2000,
                                        size_t alignment = 0x8);

    std::optional<RobloxStringInfo> scan_roblox_string(uintptr_t instance_address,
                                                       const std::string& expected_name,
                                                       size_t max_pointer_offset = 0x200,
                                                       size_t alignment = 0x8);
    std::optional<size_t> find_roblox_string_direct(uintptr_t base_address,
                                                    const std::string& expected_name,
                                                    size_t max_offset = 0x1000,
                                                    size_t alignment = 0x8);
    std::optional<size_t> find_string_direct(uintptr_t base_address,
                                             const std::string& expected_string,
                                             size_t max_offset = 0x1000, size_t alignment = 0x8,
                                             size_t max_string_length = 256);

    static DWORD find_process_id(std::string process_name);
    static HANDLE nt_open_process(DWORD process_id);
    static MODULEENTRY32W get_module_by_name(DWORD process_id, std::string module_name);
    std::string get_executable_path();
};

inline std::unique_ptr<Memory> memory;