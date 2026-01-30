#include "process/memory/memory.h"
#include <algorithm>

namespace process {
    auto Memory::read_bytes(uintptr_t address, size_t size) -> std::vector<uint8_t> {
        using tNtReadVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
        static auto fn = g_process.m_ntdll.get_export<tNtReadVirtualMemory>("NtReadVirtualMemory");

        std::vector<uint8_t> buffer(size);
        SIZE_T bytes_read = 0;

        NTSTATUS status = fn(g_process.get_handle(), reinterpret_cast<PVOID>(address),
                             buffer.data(), size, &bytes_read);

        if (!NT_SUCCESS(status) || bytes_read != size) {
            return {};
        }

        return buffer;
    }

    auto Memory::write_bytes(uintptr_t address, const std::vector<uint8_t>& data) -> bool {
        using tNtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
        static auto fn =
            g_process.m_ntdll.get_export<tNtWriteVirtualMemory>("NtWriteVirtualMemory");

        SIZE_T bytes_written = 0;

        NTSTATUS status = fn(g_process.get_handle(), reinterpret_cast<PVOID>(address),
                             const_cast<uint8_t*>(data.data()), data.size(), &bytes_written);

        return NT_SUCCESS(status) && bytes_written == data.size();
    }

    auto Memory::read_string(uintptr_t address, size_t max_length) -> std::optional<std::string> {
        auto bytes = read_bytes(address, max_length);
        if (bytes.empty()) {
            return std::nullopt;
        }

        auto null_pos = std::find(bytes.begin(), bytes.end(), '\0');
        auto str = std::string(bytes.begin(), null_pos);

        if (str.empty()) {
            return std::nullopt;
        }

        return str;
    }

    auto Memory::read_sso_string(uintptr_t address) -> std::optional<std::string> {
        if (!address) {
            return std::nullopt;
        }

        auto length = read<int32_t>(address + 0x18);
        if (!length || *length <= 0 || *length > 1024) {
            return std::nullopt;
        }

        uintptr_t data_ptr = (*length >= 16) ? *read<uintptr_t>(address) : address;

        return read_string(data_ptr, *length);
    }
} // namespace process