#pragma once
#include "process/process.h"
#include <optional>
#include <string>
#include <vector>

namespace process {
    class Memory {
      public:
        template <typename T> static auto read(uintptr_t address) -> std::optional<T> {
            using tNtReadVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
            static auto fn =
                g_process.m_ntdll.get_export<tNtReadVirtualMemory>("NtReadVirtualMemory");

            T buffer{};
            SIZE_T bytes_read = 0;

            NTSTATUS status = fn(g_process.get_handle(), reinterpret_cast<PVOID>(address), &buffer,
                                 sizeof(T), &bytes_read);

            if (!NT_SUCCESS(status) || bytes_read != sizeof(T)) {
                return std::nullopt;
            }

            return buffer;
        }

        template <typename T> static auto write(uintptr_t address, const T& value) -> bool {
            using tNtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
            static auto fn =
                g_process.m_ntdll.get_export<tNtWriteVirtualMemory>("NtWriteVirtualMemory");

            SIZE_T bytes_written = 0;

            NTSTATUS status = fn(g_process.get_handle(), reinterpret_cast<PVOID>(address),
                                 const_cast<T*>(&value), sizeof(T), &bytes_written);

            return NT_SUCCESS(status) && bytes_written == sizeof(T);
        }

        static auto read_bytes(uintptr_t address, size_t size) -> std::vector<uint8_t>;
        static auto write_bytes(uintptr_t address, const std::vector<uint8_t>& data) -> bool;
        static auto read_string(uintptr_t address, size_t max_length = 256)
            -> std::optional<std::string>;
        static auto read_sso_string(uintptr_t address) -> std::optional<std::string>;
    };
} // namespace process