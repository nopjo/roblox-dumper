#include "script.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include "spdlog/spdlog.h"

namespace dumper::stages::script {
    auto dump() -> bool {
        const auto scripts_folder =
            dumper::g_data_model.find_first_child_of_class("ReplicatedStorage")
                ->find_first_child("Scripts");

        if (!scripts_folder->is_valid()) {
            spdlog::error("Failed to find ScriptsFolder");
            return false;
        }

        const auto local_script = scripts_folder->find_first_child("LocalScript");
        const auto module_script = scripts_folder->find_first_child("ModuleScript");
        const auto module_script2 = scripts_folder->find_first_child("ModuleScript2");

        if (!local_script->is_valid() || !module_script->is_valid() ||
            !module_script2->is_valid()) {
            spdlog::error("Failed to find script instances");
            return false;
        }

        auto module_bytecode = process::helpers::find_offset_in_pointer<int>(
            module_script->get_address(), 61, 0x300, 0x100, 0x8, 0x4);

        if (!module_bytecode) {
            spdlog::error("Failed to find ModuleScript bytecode offset");
            return false;
        }

        const auto [bytecode_ptr_offset, size_offset] = *module_bytecode;

        const auto embedded2 =
            process::Memory::read<uintptr_t>(module_script2->get_address() + bytecode_ptr_offset);
        if (embedded2) {
            auto size2 = process::Memory::read<int>(*embedded2 + size_offset);
            if (!size2 || *size2 != 86) {
                spdlog::warn("ModuleScript2 verification failed (expected size 86, got {})",
                             size2.value_or(-1));
            }
        }

        g_dumper.add_offset("ModuleScript", "Bytecode", bytecode_ptr_offset);
        g_dumper.add_offset("ByteCode", "Size", size_offset);
        g_dumper.add_offset("ByteCode", "Pointer", 0x10);

        const auto local_bytecode = process::helpers::find_offset_in_pointer<int>(
            local_script->get_address(), 86, 0x300, 0x100, 0x8, 0x4);

        if (!local_bytecode) {
            spdlog::error("Failed to find LocalScript bytecode offset");
            return false;
        }

        g_dumper.add_offset("LocalScript", "Bytecode", local_bytecode->first);

        const auto module_hash = process::helpers::find_offset_in_pointer<int>(
            module_script->get_address(), 1680946276, 0x1000, 0x100, 0x8, 0x4);

        if (!module_hash) {
            spdlog::error("Failed to find ModuleScript hash offset");
            return false;
        }

        g_dumper.add_offset("ModuleScript", "Hash", module_hash->first);

        const auto local_hash = process::helpers::find_offset_in_pointer<int>(
            local_script->get_address(), 1680946276, 0x1000, 0x100, 0x8, 0x4);

        if (!local_hash) {
            spdlog::error("Failed to find LocalScript hash offset");
            return false;
        }

        g_dumper.add_offset("LocalScript", "Hash", local_hash->first);

        return true;
    }
} // namespace dumper::stages::script