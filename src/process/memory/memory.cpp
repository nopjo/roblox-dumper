#include "process/memory/memory.h"
#include "memory.h"
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

        auto length = read<int32_t>(address + 0x10);
        if (!length || *length <= 0 || *length > 1024) {
            return std::nullopt;
        }

        uintptr_t data_ptr = (*length >= 16) ? *read<uintptr_t>(address) : address;

        return read_string(data_ptr, *length);
    }

    auto Memory::scan_string(const std::string& target, std::string_view section)
        -> std::vector<uintptr_t> {
        std::vector<uintptr_t> matches;

        if (target.empty()) {
            return matches;
        }

        if (!section.empty()) {
            auto sec = g_process.get_section(section);
            if (!sec) {
                return matches;
            }

            auto buffer = read_bytes(sec->first, sec->second);
            if (buffer.size() < target.size()) {
                return matches;
            }

            for (size_t offset = 0; offset <= buffer.size() - target.size(); offset++) {
                if (std::memcmp(buffer.data() + offset, target.data(), target.size()) == 0) {
                    matches.push_back(sec->first + offset);
                }
            }

            return matches;
        }

        MEMORY_BASIC_INFORMATION mbi{};
        uintptr_t current = g_process.get_module_base();

        while (VirtualQueryEx(g_process.get_handle(), reinterpret_cast<LPCVOID>(current), &mbi,
                              sizeof(mbi)) == sizeof(mbi)) {
            const uintptr_t region_start = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
            const uintptr_t region_end = region_start + mbi.RegionSize;

            if (mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_GUARD) &&
                !(mbi.Protect & PAGE_NOACCESS)) {
                auto buffer = read_bytes(region_start, mbi.RegionSize);

                if (buffer.size() >= target.size()) {
                    for (size_t offset = 0; offset <= buffer.size() - target.size(); offset++) {
                        if (std::memcmp(buffer.data() + offset, target.data(), target.size()) ==
                            0) {
                            matches.push_back(region_start + offset);
                        }
                    }
                }
            }

            current = region_end;
        }

        return matches;
    }

} // namespace process
