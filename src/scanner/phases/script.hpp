#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/offsets.hpp"
#include "settings.hpp"
#include "utils/offset_registry.hpp"
#include <chrono>
#include <thread>

namespace scanner::phases {
    inline bool script(Context& ctx) {
        const auto replicated_storage =
            sdk::instance_t(ctx.data_model).find_first_child("ReplicatedStorage");

        if (!replicated_storage.is_valid()) {
            return false;
        }

        const auto scripts = replicated_storage.find_first_child("scripts");

        if (!scripts.is_valid()) {
            return false;
        }

        const auto module_folder = scripts.find_first_child("module");

        if (!module_folder.is_valid()) {
            return false;
        }

        const auto local_folder = scripts.find_first_child("local");

        if (!local_folder.is_valid()) {
            return false;
        }

        const auto module_script = module_folder.find_first_child("ModuleScript");

        if (!module_script.is_valid()) {
            return false;
        } // 61

        const auto local_script = local_folder.find_first_child("LocalScript");

        if (!local_script.is_valid()) {
            return false;
        } // 86

        const auto module_script2 = module_folder.find_first_child("ModuleScript2");

        if (!module_script2.is_valid()) {
            return false;
        } // 86

        // module_script_bytecode
        for (size_t off = 0; off < 0x1000; off += 0x8) {
            const auto embedded = memory->read(module_script.address + off);
            if (!embedded)
                continue;

            const auto embedded2 = memory->read(module_script2.address + off);
            if (!embedded2)
                continue;

            const auto size = memory->find_verified_offset<int>({embedded, embedded2}, {61, 86});

            if (size) {
                offset_registry.add("ByteCode", "Size", *size);
                offset_registry.add("ByteCode", "Pointer", 0x10); // hardcoded for now
                offset_registry.add("ModuleScript", "Bytecode", off);
                break;
            }
        }

        // local_script_bytecode
        for (size_t off = 0; off < 0x1000; off += 0x8) {
            const auto embedded = memory->read(local_script.address + off);
            if (!embedded)
                continue;

            const auto size = memory->read<int>(embedded + offset_registry.get("ByteCode", "Size"));

            if (size == 86) {
                offset_registry.add("LocalScript", "Bytecode", off);
                break;
            }
        }

        // module_script_hash
        for (size_t off = 0; off < 0x1000; off += 0x8) {
            const auto a = memory->read(module_script.address + off);
            const auto hash = memory->read<int>(a + 0x0);
            if (hash == 1680946276) {
                offset_registry.add("ModuleScript", "Hash", off);
                break;
            }
        }

        // local_script_hash
        for (size_t off = 0; off < 0x1000; off += 0x8) {
            const auto a = memory->read(local_script.address + off);
            const auto hash = memory->read<int>(a + 0x0);
            if (hash == 1680946276) {
                offset_registry.add("LocalScript", "Hash", off);
                break;
            }
        }

        return true;
    }

} // namespace scanner::phases