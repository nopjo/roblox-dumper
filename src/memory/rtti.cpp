#include "rtti.h"
#include "memory.h"
#include "utils/logger.hpp"
#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>
#include <thread>
#include <unordered_set>

std::optional<RttiInfo> Memory::scan_rtti(uintptr_t address) {
    uintptr_t vtable = read<uintptr_t>(address);
    if (!vtable)
        return std::nullopt;

    uintptr_t col_ptr = read<uintptr_t>(vtable - 0x8);
    if (!col_ptr)
        return std::nullopt;

    DWORD signature = read<DWORD>(col_ptr);
    if (signature > 1)
        return std::nullopt;

    bool is_x64 = (signature == 1);
    uintptr_t module_base = 0;
    if (is_x64) {
        int self_offset = read<int>(col_ptr + 0x14);
        module_base = col_ptr - self_offset;
    }

    if (is_x64) {
        struct COL_x64 {
            DWORD signature;
            DWORD offset;
            DWORD cdOffset;
            int pTypeDescriptor;
            int pClassDescriptor;
            int pSelf;
        } col;

        if (!read_raw(col_ptr, &col, sizeof(col)))
            return std::nullopt;

        RttiInfo info{};
        info.type_descriptor = module_base + col.pTypeDescriptor;
        info.class_hierarchy_descriptor = module_base + col.pClassDescriptor;

        TypeDescriptor td;
        if (!read_raw(info.type_descriptor, &td, sizeof(td)))
            return std::nullopt;

        std::string raw_name(td.name, strnlen(td.name, 255));
        if (raw_name.size() > 4 && raw_name.substr(0, 4) == ".?AV")
            raw_name = raw_name.substr(4);
        size_t at_pos = raw_name.find("@@");
        if (at_pos != std::string::npos)
            raw_name = raw_name.substr(0, at_pos);
        info.name = raw_name;

        return info;
    } else {
        RTTICompleteObjectLocator col;
        if (!read_raw(col_ptr, &col, sizeof(col)))
            return std::nullopt;

        RttiInfo info{};
        info.type_descriptor = col.pTypeDescriptor;
        info.class_hierarchy_descriptor = col.pClassDescriptor;

        TypeDescriptor td;
        if (!read_raw(info.type_descriptor, &td, sizeof(td)))
            return std::nullopt;

        std::string raw_name(td.name, strnlen(td.name, 255));
        if (raw_name.size() > 4 && raw_name.substr(0, 4) == ".?AV")
            raw_name = raw_name.substr(4);
        size_t at_pos = raw_name.find("@@");
        if (at_pos != std::string::npos)
            raw_name = raw_name.substr(0, at_pos);
        info.name = raw_name;

        return info;
    }
}

std::optional<size_t> Memory::find_rtti_offset(uintptr_t base_address,
                                               const std::string& target_class, size_t max_offset,
                                               size_t alignment) {
    for (size_t offset = 0; offset < max_offset; offset += alignment) {
        uintptr_t current_address = base_address + offset;
        uintptr_t pointer_value = read<uintptr_t>(current_address);

        if (!pointer_value || pointer_value < 0x10000)
            continue;

        auto rtti = scan_rtti(pointer_value);
        if (rtti && rtti->name == target_class)
            return offset;
    }
    return std::nullopt;
}

std::optional<size_t> Memory::find_rtti_offset_nth(uintptr_t base_address,
                                                    const std::string& target_class, size_t nth_index,
                                                    size_t max_offset, size_t alignment) {
    size_t match_count = 0;
    for (size_t offset = 0; offset < max_offset; offset += alignment) {
        uintptr_t current_address = base_address + offset;
        uintptr_t pointer_value = read<uintptr_t>(current_address);

        if (!pointer_value || pointer_value < 0x10000)
            continue;

        auto rtti = scan_rtti(pointer_value);
        if (rtti && rtti->name == target_class) {
            if (match_count == nth_index) {
                return offset;
            }
            match_count++;
        }
    }
    return std::nullopt;
}

std::optional<uintptr_t> Memory::find_pointer_by_rtti(const std::string& target_rtti,
                                                      size_t scan_size, size_t alignment) {
    uintptr_t module_base = base_address();
    size_t start_offset = 0;

    auto data_sec = get_section_range(".data");
    if (data_sec) {
        start_offset = data_sec->first - module_base;
        scan_size = data_sec->second;
    } else {
        MODULEENTRY32W mod = get_module_by_name(process_id, process_name);
        if (mod.modBaseSize != 0) {
            scan_size = (std::min)(scan_size, static_cast<size_t>(mod.modBaseSize));
        }
    }

    for (size_t offset = start_offset; offset < start_offset + scan_size; offset += alignment) {
        uintptr_t potential_ptr = read<uintptr_t>(module_base + offset);
        if (potential_ptr < 0x10000 || potential_ptr > 0x7FFFFFFFFFFF)
            continue;

        auto rtti = scan_rtti(potential_ptr);
        if (rtti && rtti->name == target_rtti) {
            return offset;
        }
    }

    LOG_ERR("Failed to find {} via RTTI (single-threaded scan over 0x{:X} bytes)", target_rtti,
            scan_size);
    return std::nullopt;
}

std::vector<RttiInfo> Memory::scan_all_rtti(uintptr_t base_address, size_t max_offset,
                                            size_t alignment) {
    std::vector<RttiInfo> results;
    for (size_t offset = 0; offset < max_offset; offset += alignment) {
        uintptr_t current_address = base_address + offset;
        uintptr_t pointer_value = read<uintptr_t>(current_address);

        if (!pointer_value || pointer_value < 0x10000)
            continue;

        auto rtti = scan_rtti(pointer_value);
        if (rtti) {
            rtti->type_descriptor = offset;
            results.push_back(*rtti);
        }
    }
    return results;
}
