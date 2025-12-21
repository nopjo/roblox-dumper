#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool highlight(Context& ctx) {
        const auto replicated_storage =
            sdk::instance_t(ctx.data_model).find_first_child("ReplicatedStorage");
        if (!replicated_storage.is_valid()) {
            LOG_ERR("Failed to find ReplicatedStorage");
            return false;
        }

        const auto highlight = replicated_storage.find_first_child_of_class("Highlight");
        if (!highlight.is_valid()) {
            LOG_ERR("Failed to find Highlight in 'ReplicatedStorage'");
            return false;
        }

        const auto fill_transparency_offset =
            memory->find_verified_offset_float({highlight.address}, {0.456f}, 0x800, 0x4);

        if (!fill_transparency_offset) {
            LOG_ERR("Failed to find FillTransparency offset for Highlight");
            return false;
        }

        offset_registry.add("Highlight", "FillTransparency", *fill_transparency_offset);

        const auto outline_transparency_offset =
            memory->find_verified_offset_float({highlight.address}, {0.652f}, 0x800, 0x4);

        if (!outline_transparency_offset) {
            LOG_ERR("Failed to find OutlineTransparency offset for Highlight");
            return false;
        }

        offset_registry.add("Highlight", "OutlineTransparency", *outline_transparency_offset);

        constexpr float FILL_COLOR_R = 27.0f / 255.0f;
        constexpr float FILL_COLOR_G = 185.0f / 255.0f;
        constexpr float FILL_COLOR_B = 27.0f / 255.0f;

        auto fill_color_offset =
            memory->find_verified_offset_float({highlight.address}, {FILL_COLOR_R}, 0x800, 0x4);

        if (fill_color_offset) {
            float g = memory->read<float>(highlight.address + *fill_color_offset + 4);
            float b = memory->read<float>(highlight.address + *fill_color_offset + 8);
            if (std::abs(g - FILL_COLOR_G) < 0.01f && std::abs(b - FILL_COLOR_B) < 0.01f) {
                offset_registry.add("Highlight", "FillColor", *fill_color_offset);
            } else {
                LOG_ERR("Failed to verify FillColor G/B channels for Highlight");
            }
        } else {
            LOG_ERR("Failed to find FillColor offset for Highlight");
        }

        constexpr float OUTLINE_COLOR_R = 68.0f / 255.0f;
        constexpr float OUTLINE_COLOR_G = 168.0f / 255.0f;
        constexpr float OUTLINE_COLOR_B = 211.0f / 255.0f;

        auto outline_color_offset =
            memory->find_verified_offset_float({highlight.address}, {OUTLINE_COLOR_R}, 0x800, 0x4);

        if (outline_color_offset) {
            float g = memory->read<float>(highlight.address + *outline_color_offset + 4);
            float b = memory->read<float>(highlight.address + *outline_color_offset + 8);
            if (std::abs(g - OUTLINE_COLOR_G) < 0.01f && std::abs(b - OUTLINE_COLOR_B) < 0.01f) {
                offset_registry.add("Highlight", "OutlineColor", *outline_color_offset);
            } else {
                LOG_ERR("Failed to verify OutlineColor G/B channels for Highlight");
            }
        } else {
            LOG_ERR("Failed to find OutlineColor offset for Highlight");
        }

        control::Controller controller("http://localhost:8000");
        constexpr int SLEEP_MS = 200;

        controller.set_highlight_depth_mode(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

        std::vector<int32_t> depth_mode_values = {0, 1, 0};
        auto depth_mode_offsets = memory->find_offsets_with_snapshots<int32_t>(
            highlight.address, depth_mode_values,
            [&](size_t i) {
                controller.set_highlight_depth_mode(depth_mode_values[i]);
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            },
            0x800, 0x4, SLEEP_MS);

        if (!depth_mode_offsets.empty()) {
            offset_registry.add("Highlight", "DepthMode", depth_mode_offsets[0]);
        } else {
            LOG_ERR("Failed to find DepthMode offset for Highlight");
        }

        return true;
    }

} // namespace scanner::phases