#include "process/rtti/rtti.h"
#include "process/memory/memory.h"
#include "rtti.h"
#include <cstring>

namespace process {

    auto Rtti::scan_rtti(uintptr_t address) -> std::optional<RttiInfo> {
        auto vtable = Memory::read<uintptr_t>(address);
        if (!vtable) {
            return std::nullopt;
        }

        auto col_ptr = Memory::read<uintptr_t>(*vtable - 0x8);
        if (!col_ptr) {
            return std::nullopt;
        }

        auto signature = Memory::read<uint32_t>(*col_ptr);
        if (!signature || *signature > 1) {
            return std::nullopt;
        }

        bool is_x64 = (*signature == 1);
        if (!is_x64) {
            return std::nullopt;
        }

        auto self_offset = Memory::read<int>(*col_ptr + 0x14);
        if (!self_offset) {
            return std::nullopt;
        }

        uintptr_t module_base = *col_ptr - *self_offset;

        auto col_bytes = Memory::read_bytes(*col_ptr, sizeof(RttiCompleteObjectLocatorX64));
        if (col_bytes.empty()) {
            return std::nullopt;
        }

        RttiCompleteObjectLocatorX64 col;
        std::memcpy(&col, col_bytes.data(), sizeof(col));

        RttiInfo info{};
        info.type_descriptor = module_base + col.type_descriptor_offset;
        info.class_hierarchy_descriptor = module_base + col.class_descriptor_offset;

        auto td_bytes = Memory::read_bytes(info.type_descriptor, sizeof(TypeDescriptor));
        if (td_bytes.empty()) {
            return std::nullopt;
        }

        TypeDescriptor td;
        std::memcpy(&td, td_bytes.data(), sizeof(td));

        std::string raw_name(td.name, strnlen(td.name, 255));

        if (raw_name.size() > 4 && raw_name.substr(0, 4) == ".?AV") {
            raw_name = raw_name.substr(4);
        }

        size_t at_pos = raw_name.find("@@");
        if (at_pos != std::string::npos) {
            raw_name = raw_name.substr(0, at_pos);
        }

        info.name = raw_name;

        return info;
    }

    auto Rtti::find(uintptr_t base_address, const std::string& target_class, size_t max_offset,
                    size_t alignment) -> std::optional<size_t> {
        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            uintptr_t current_address = base_address + offset;
            auto pointer_value = Memory::read<uintptr_t>(current_address);

            if (!pointer_value || *pointer_value < 0x10000) {
                continue;
            }

            auto rtti = scan_rtti(*pointer_value);
            if (rtti && rtti->name == target_class) {
                return offset;
            }
        }

        return std::nullopt;
    }

    auto Rtti::find_all(uintptr_t base_address, const std::string& target_class, size_t max_offset,
                        size_t alignment) -> std::vector<size_t> {
        std::vector<size_t> matches;

        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            uintptr_t current_address = base_address + offset;
            auto pointer_value = Memory::read<uintptr_t>(current_address);

            if (!pointer_value || *pointer_value < 0x10000) {
                continue;
            }

            auto rtti = scan_rtti(*pointer_value);
            if (rtti && rtti->name == target_class) {
                matches.push_back(offset);
            }
        }

        return matches;
    }

} // namespace process