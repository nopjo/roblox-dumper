#pragma once
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <chrono>
#include <thread>


namespace scanner::phases {

    inline bool color_correction_effect(Context& ctx) {
        const auto lighting = sdk::instance_t(ctx.data_model).find_first_child("Lighting");
        if (!lighting.is_valid()) {
            LOG_ERR("Failed to find Lighting");
            return false;
        }

        const auto color_correct_inst = lighting.find_first_child_of_class("ColorCorrectionEffect");
        if (!lighting.is_valid()) {
            LOG_ERR("Failed to find ColorCorrectionEffect in 'Lighting'");
            return false;
        }

        control::Controller controller("http://localhost:8000");

        const auto brightness_offset =
            memory->find_verified_offset_float({color_correct_inst.address}, {-0.67f}, 0x200, 0x2);

        if (!brightness_offset) {
            LOG_ERR("Failed to find Brightness offset for ColorCorrectEffect");
            return false;
        }

        offset_registry.add("ColorCorrectionEffect", "Brightness", *brightness_offset);

        const auto contrast_offset =
            memory->find_verified_offset_float({color_correct_inst.address}, {1.123f}, 0x200, 0x2);

        if (!contrast_offset) {
            LOG_ERR("Failed to find Contrast offset for ColorCorrectEffect");
            return false;
        }

        offset_registry.add("ColorCorrectionEffect", "Contrast", *contrast_offset);

        const auto saturation_offset =
            memory->find_verified_offset_float({color_correct_inst.address}, {0.269f}, 0x200, 0x2);

        if (!saturation_offset) {
            LOG_ERR("Failed to find Saturation offset for ColorCorrectEffect");
            return false;
        }

        offset_registry.add("ColorCorrectionEffect", "Saturation", *saturation_offset);

        constexpr float TINT_R = 255.0f / 255.0f;
        constexpr float TINT_G = 118.0f / 255.0f;
        constexpr float TINT_B = 117.0f / 255.0f;

        auto tint_offset = memory->find_verified_offset_float({color_correct_inst.address},
                                                              {TINT_R}, 0x600, 0x4);

        if (tint_offset) {
            float g = memory->read<float>(color_correct_inst.address + *tint_offset + 4);
            float b = memory->read<float>(color_correct_inst.address + *tint_offset + 8);
            if (std::abs(g - TINT_G) < 0.01f && std::abs(b - TINT_B) < 0.01f) {
                offset_registry.add("ColorCorrectionEffect", "TintColor", *tint_offset);
            } else {
                LOG_ERR("Failed to verify TintColor G/B channels for ColorCorrectionEffect");
            }
        } else {
            LOG_ERR("Failed to find TintColor offset for ColorCorrectionEffect");
        }

        return true;
    }

} // namespace scanner::phases