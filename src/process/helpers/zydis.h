#pragma once
#include "process/memory/memory.h"
#include <Zydis/Zydis.h>


namespace process::helpers::zydis {

    inline auto is_cmp_mem_zero(const ZydisDecodedInstruction& inst, const ZydisDecodedOperand* ops,
                                ZydisRegister base, size_t min_disp = 0) -> bool {
        return inst.mnemonic == ZYDIS_MNEMONIC_CMP && ops[0].type == ZYDIS_OPERAND_TYPE_MEMORY &&
               ops[0].mem.base == base && ops[0].mem.disp.has_displacement &&
               ops[0].mem.disp.value > min_disp && ops[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE &&
               ops[1].imm.value.u == 0;
    }

    inline auto is_movzx_mem(const ZydisDecodedInstruction& inst, const ZydisDecodedOperand* ops,
                             ZydisRegister base, size_t min_disp = 0) -> bool {
        return inst.mnemonic == ZYDIS_MNEMONIC_MOVZX && ops[1].type == ZYDIS_OPERAND_TYPE_MEMORY &&
               ops[1].mem.base == base && ops[1].mem.disp.has_displacement &&
               ops[1].mem.disp.value > min_disp;
    }

    inline auto find_function_start(uintptr_t addr) -> std::optional<uintptr_t> {
        constexpr size_t search_size = 0x1000;
        uintptr_t region_start = addr - search_size;
        auto buffer = process::Memory::read_bytes(region_start, search_size);
        if (buffer.empty())
            return std::nullopt;

        for (size_t i = buffer.size() - 1; i > 0; i--) {
            if (buffer[i] == 0xCC) {
                while (i < buffer.size() && buffer[i] == 0xCC)
                    i++;
                return region_start + i;
            }
        }

        return std::nullopt;
    }

    inline auto resolve_rip_mov_store(uintptr_t addr, size_t pre = 0x50, size_t total = 0x200)
        -> std::optional<uintptr_t> {
        auto buffer = process::Memory::read_bytes(addr - pre, total);
        if (buffer.empty())
            return std::nullopt;

        ZydisDecoder decoder;
        ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

        size_t offset = 0;
        while (offset < buffer.size()) {
            ZydisDecodedInstruction insn;
            ZydisDecodedOperand ops[ZYDIS_MAX_OPERAND_COUNT];

            if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, buffer.data() + offset,
                                                     buffer.size() - offset, &insn, ops))) {
                offset++;
                continue;
            }

            if (insn.mnemonic == ZYDIS_MNEMONIC_MOV && ops[0].type == ZYDIS_OPERAND_TYPE_MEMORY &&
                ops[0].mem.base == ZYDIS_REGISTER_RIP &&
                ops[1].type == ZYDIS_OPERAND_TYPE_REGISTER) {
                ZyanU64 absolute = 0;
                if (ZYAN_SUCCESS(
                        ZydisCalcAbsoluteAddress(&insn, &ops[0], addr - pre + offset, &absolute))) {
                    return static_cast<uintptr_t>(absolute);
                }
            }

            offset += insn.length;
        }

        return std::nullopt;
    }

} // namespace process::helpers::zydis