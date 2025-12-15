#pragma once
#include "memory/memory.h"
#include "scanner.hpp"
#include "utils/offset_registry.hpp"
#include <cmath>

namespace scanner::phases {

    inline bool visual_engine(Context& ctx) {
        const auto visual_engine_pointer =
            memory->find_pointer_by_rtti("VisualEngine@Graphics@RBX");
        if (!visual_engine_pointer) {
            LOG_ERR("Failed to find VisualEngine Pointer");
            return false;
        }

        ctx.visual_engine =
            memory->read<uintptr_t>(memory->base_address() + *visual_engine_pointer);
        if (!ctx.visual_engine) {
            LOG_ERR("Failed to read VisualEngine pointer at module offset 0x{:X} (addr 0x{:X})",
                    *visual_engine_pointer, memory->base_address() + *visual_engine_pointer);
            return false;
        }

        offset_registry.add("VisualEngine", "Pointer", *visual_engine_pointer);

        auto render_view_offset =
            memory->find_rtti_offset(ctx.visual_engine, "RenderView@Graphics@RBX");
        if (!render_view_offset) {
            LOG_ERR("Failed to find RenderView offset");
            return false;
        }
        offset_registry.add("VisualEngine", "RenderView", *render_view_offset);

        auto render_view = memory->read<uintptr_t>(ctx.visual_engine + *render_view_offset);
        if (!render_view) {
            LOG_ERR("Failed to read RenderView pointer");
            return false;
        }

        auto invalidate_lighting =
            memory->find_value_offset<uint32_t>(render_view, 257, 0x1000, 0x2);
        if (!invalidate_lighting) {
            LOG_ERR("Failed to find InvalidateLighting offset");
            return false;
        }
        offset_registry.add("RenderView", "InvalidateLighting", *invalidate_lighting);

        auto dimensions_x_offset =
            memory->find_value_offset<float>(ctx.visual_engine, 800.0f, 0x1000, 0x4);
        if (dimensions_x_offset) {
            float y_value = memory->read<float>(ctx.visual_engine + *dimensions_x_offset + 0x4);
            if (std::abs(y_value - 599.0f) < 1.0f) {
                offset_registry.add("VisualEngine", "WindowDimensions", *dimensions_x_offset);
            } else {
                LOG_ERR("Failed to verify WindowDimensions Y value (got {})", y_value);
            }
        } else {
            LOG_ERR("Failed to find WindowDimensions offset");
        }

        return true;
    }

} // namespace scanner::phases