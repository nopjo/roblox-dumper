#include "rtti.h"
#include "memory.h"
#include "utils/config.hpp"
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

    RttiInfo info;

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

        info.type_descriptor = module_base + col.pTypeDescriptor;
        info.class_hierarchy_descriptor = module_base + col.pClassDescriptor;

        TypeDescriptor td;
        if (read_raw(info.type_descriptor, &td, sizeof(td))) {
            std::string raw_name(td.name, strnlen(td.name, 255));
            if (raw_name.size() > 4 && raw_name.substr(0, 4) == ".?AV")
                raw_name = raw_name.substr(4);
            size_t at_pos = raw_name.find("@@");
            if (at_pos != std::string::npos)
                raw_name = raw_name.substr(0, at_pos);
            info.name = raw_name;
        }

        RTTIClassHierarchyDescriptor chd;
        if (read_raw(info.class_hierarchy_descriptor, &chd, sizeof(chd))) {
            uintptr_t base_array = module_base + chd.pBaseClassArray;
            for (DWORD i = 0; i < chd.numBaseClasses && i < 32; i++) {
                int base_offset = read<int>(base_array + i * sizeof(int));
                uintptr_t base_ptr = module_base + base_offset;

                RTTIBaseClassDescriptor bcd;
                if (read_raw(base_ptr, &bcd, sizeof(bcd))) {
                    uintptr_t base_type = module_base + bcd.pTypeDescriptor;
                    TypeDescriptor base_td;
                    if (read_raw(base_type, &base_td, sizeof(base_td))) {
                        std::string base_name(base_td.name, strnlen(base_td.name, 255));
                        if (base_name.size() > 4 && base_name.substr(0, 4) == ".?AV")
                            base_name = base_name.substr(4);
                        size_t at_pos = base_name.find("@@");
                        if (at_pos != std::string::npos)
                            base_name = base_name.substr(0, at_pos);
                        info.base_classes.push_back(base_name);
                    }
                }
            }
        }
    } else {
        RTTICompleteObjectLocator col;
        if (!read_raw(col_ptr, &col, sizeof(col)))
            return std::nullopt;

        info.type_descriptor = col.pTypeDescriptor;
        info.class_hierarchy_descriptor = col.pClassDescriptor;

        TypeDescriptor td;
        if (read_raw(info.type_descriptor, &td, sizeof(td))) {
            std::string raw_name(td.name, strnlen(td.name, 255));
            if (raw_name.size() > 4 && raw_name.substr(0, 4) == ".?AV")
                raw_name = raw_name.substr(4);
            size_t at_pos = raw_name.find("@@");
            if (at_pos != std::string::npos)
                raw_name = raw_name.substr(0, at_pos);
            info.name = raw_name;
        }

        RTTIClassHierarchyDescriptor chd;
        if (read_raw(info.class_hierarchy_descriptor, &chd, sizeof(chd))) {
            for (DWORD i = 0; i < chd.numBaseClasses && i < 32; i++) {
                uintptr_t base_ptr = read<uintptr_t>(chd.pBaseClassArray + i * sizeof(uintptr_t));
                RTTIBaseClassDescriptor bcd;
                if (read_raw(base_ptr, &bcd, sizeof(bcd))) {
                    TypeDescriptor base_td;
                    if (read_raw(bcd.pTypeDescriptor, &base_td, sizeof(base_td))) {
                        std::string base_name(base_td.name, strnlen(base_td.name, 255));
                        if (base_name.size() > 4 && base_name.substr(0, 4) == ".?AV")
                            base_name = base_name.substr(4);
                        size_t at_pos = base_name.find("@@");
                        if (at_pos != std::string::npos)
                            base_name = base_name.substr(0, at_pos);
                        info.base_classes.push_back(base_name);
                    }
                }
            }
        }
    }

    return info;
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

std::optional<uintptr_t> Memory::find_pointer_by_rtti(const std::string& target_rtti,
                                                      size_t scan_size, size_t alignment) {
    uintptr_t base = base_address();

    MODULEENTRY32W mod = get_module_by_name(process_id, process_name);
    if (mod.modBaseSize != 0) {
        scan_size = (std::min)(scan_size, static_cast<size_t>(mod.modBaseSize));
    }

    const bool use_mt = config::get().multithreaded_rtti_scan;

    if (!use_mt) {
        for (size_t offset = 0; offset < scan_size; offset += alignment) {
            uintptr_t potential_ptr = read<uintptr_t>(base + offset);
            if (potential_ptr < 0x10000 || potential_ptr > 0x7FFFFFFFFFFF)
                continue;

            auto rtti = scan_rtti(potential_ptr);
            if (rtti && rtti->name == target_rtti) {
                return offset;
            }
        }
    } else {
        unsigned int hw_threads = std::thread::hardware_concurrency();
        if (hw_threads == 0) {
            hw_threads = 1;
        }

        const unsigned int num_threads = hw_threads;
        const size_t chunk_size = scan_size / num_threads;

        std::vector<std::thread> threads;
        std::atomic<bool> found{false};
        std::atomic<uintptr_t> result_offset{0};

        auto scan_chunk = [&](size_t start_offset, size_t end_offset) {
            for (size_t offset = start_offset; offset < end_offset && !found.load();
                 offset += alignment) {
                uintptr_t potential_ptr = read<uintptr_t>(base + offset);
                if (potential_ptr < 0x10000 || potential_ptr > 0x7FFFFFFFFFFF)
                    continue;

                auto rtti = scan_rtti(potential_ptr);
                if (rtti && rtti->name == target_rtti) {
                    bool expected = false;
                    if (found.compare_exchange_strong(expected, true)) {
                        result_offset.store(offset);
                    }
                    return;
                }
            }
        };

        for (unsigned int i = 0; i < num_threads; i++) {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads - 1) ? scan_size : (i + 1) * chunk_size;
            threads.emplace_back(scan_chunk, start, end);
        }

        for (auto& thread : threads)
            thread.join();

        if (found.load())
            return result_offset.load();
    }

    LOG_ERR("Failed to find {} via RTTI ({} scan over 0x{:X} bytes)", target_rtti,
            use_mt ? "multi-threaded" : "single-threaded", scan_size);
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
