#include "script_context.h"
#include "dumper/dumper.h"
#include "process/helpers/zydis.h"
#include "process/memory/memory.h"
#include "process/process.h"
#include "process/xref/xref.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::script_context {

    auto dump() -> bool {
        const auto string_results = process::Memory::scan_string(
            "Cannot require a RobloxScript module from a non RobloxScript context", ".rdata");

        if (string_results.empty()) {
            spdlog::warn("script_context: string not found");
            return false;
        }

        const auto xrefs = process::g_xref.scan(string_results.front());
        if (xrefs.empty()) {
            spdlog::warn("script_context: no xrefs found");
            return false;
        }

        const uintptr_t xref = xrefs.front();

        const auto xrefs2 = process::g_xref.scan(xref);
        if (xrefs2.empty()) {
            spdlog::warn("script_context: no second xrefs found");
            return false;
        }

        const uintptr_t xref2 = xrefs2.front();

        constexpr size_t search_back = 0x100;
        const uintptr_t search_start = xref2 - search_back;

        auto buffer = process::Memory::read_bytes(search_start, search_back);
        if (buffer.empty()) {
            spdlog::error("script_context: failed to read buffer");
            return false;
        }

        const auto require_bypass = process::g_xref.instruction_scan(
            search_start, buffer,
            [](const ZydisDecodedInstruction& instr, const ZydisDecodedOperand* operands) {
                return process::helpers::zydis::is_cmp_mem_zero(instr, operands, ZYDIS_REGISTER_RDI,
                                                                0x100);
            });

        if (!require_bypass) {
            spdlog::error("script_context: failed to find RequireBypass");
            return false;
        }

        const auto require_bypass_offset = require_bypass->operands[0].mem.disp.value;
        g_dumper.add_offset("ScriptContext", "RequireBypass", require_bypass_offset);

        const auto is_roblox_script = process::g_xref.instruction_scan(
            search_start, buffer,
            [](const ZydisDecodedInstruction& instr, const ZydisDecodedOperand* operands) {
                return process::helpers::zydis::is_movzx_mem(instr, operands, ZYDIS_REGISTER_RAX,
                                                             0x100);
            });

        if (!is_roblox_script) {
            spdlog::error("script_context: failed to find IsRobloxScript");
            return false;
        }

        const auto is_roblox_script_offset = is_roblox_script->operands[1].mem.disp.value;
        g_dumper.add_offset("ModuleScript", "IsRobloxScript", is_roblox_script_offset);

        return true;
    }

} // namespace dumper::stages::script_context