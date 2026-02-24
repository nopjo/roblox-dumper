#pragma once
#include <Zydis/Zydis.h>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

namespace process {

    struct InstructionMatch {
        uintptr_t address;
        ZydisDecodedInstruction instruction;
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
    };

    class Xref {
      public:
        Xref();

        auto scan(uintptr_t address) const -> std::vector<uintptr_t>;

        auto instruction_scan(
            uintptr_t start, const std::vector<uint8_t>& buffer,
            const std::function<bool(const ZydisDecodedInstruction&, const ZydisDecodedOperand*)>&
                predicate) const -> std::optional<InstructionMatch>;

      private:
        auto decode(const uint8_t* buffer, size_t length, ZydisDecodedInstruction& out_instruction,
                    ZydisDecodedOperand* out_operands) const -> bool;

      private:
        ZydisDecoder m_decoder;
    };

    inline Xref g_xref;

} // namespace process