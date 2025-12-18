#pragma once
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool atmosphere(Context& ctx) {
        const auto lighting = sdk::instance_t(ctx.data_model).find_first_child("Lighting");
        if (!lighting.is_valid()) {
            LOG_ERR("Failed to find Lighting");
            return false;
        }

        const auto atmosphere_inst = lighting.find_first_child_of_class("Atmosphere");
        if (!atmosphere_inst.is_valid()) {
            LOG_ERR("Failed to find Atmosphere in 'Lighting'");
            return false;
        }

        const auto density_offset =
            memory->find_verified_offset_float({atmosphere_inst.address}, {0.677f}, 0x1000, 0x2);

        if (!density_offset) {
            LOG_ERR("Failed to find Density offset");
            return false;
        }

        offset_registry.add("Atmosphere", "Density", *density_offset);

        // holy fuck nice var name
        const auto offset_offset =
            memory->find_verified_offset_float({atmosphere_inst.address}, {0.385f}, 0x1000, 0x2);

        if (!density_offset) {
            LOG_ERR("Failed to find Offset offset");
            return false;
        }

        offset_registry.add("Atmosphere", "Offset", *offset_offset);

        const auto glare_offset =
            memory->find_verified_offset_float({atmosphere_inst.address}, {5.13f}, 0x1000, 0x2);

        if (!glare_offset) {
            LOG_ERR("Failed to find Glare offset");
            return false;
        }

        offset_registry.add("Atmosphere", "Glare", *glare_offset);

        const auto haze_offset =
            memory->find_verified_offset_float({atmosphere_inst.address}, {7.78f}, 0x1000, 0x2);

        if (!haze_offset) {
            LOG_ERR("Failed to find Haze offset");
            return false;
        }

        offset_registry.add("Atmosphere", "Haze", *haze_offset);

        constexpr float ATM_COLOR_R = 108.0f / 255.0f;
        constexpr float ATM_COLOR_G = 92.0f / 255.0f;
        constexpr float ATM_COLOR_B = 231.0f / 255.0f;

        auto atm_color_offset = memory->find_verified_offset_float({atmosphere_inst.address},
                                                                   {ATM_COLOR_R}, 0x600, 0x4);

        if (atm_color_offset) {
            float g = memory->read<float>(atmosphere_inst.address + *atm_color_offset + 4);
            float b = memory->read<float>(atmosphere_inst.address + *atm_color_offset + 8);
            if (std::abs(g - ATM_COLOR_G) < 0.01f && std::abs(b - ATM_COLOR_B) < 0.01f) {
                offset_registry.add("Atmosphere", "Color", *atm_color_offset);
            } else {
                LOG_ERR("Failed to verify Atmosphere Color G/B channels");
            }
        } else {
            LOG_ERR("Failed to find Atmosphere Color offset");
        }

        constexpr float DECAY_R = 106.0f / 255.0f;
        constexpr float DECAY_G = 176.0f / 255.0f;
        constexpr float DECAY_B = 76.0f / 255.0f;

        std::optional<size_t> decay_offset_final;
        for (size_t offset = 0; offset < 0x600; offset += 0x4) {
            if (atm_color_offset && offset == *atm_color_offset)
                continue;

            float r = memory->read<float>(atmosphere_inst.address + offset);
            if (std::abs(r - DECAY_R) < 0.01f) {
                float g = memory->read<float>(atmosphere_inst.address + offset + 4);
                float b = memory->read<float>(atmosphere_inst.address + offset + 8);
                if (std::abs(g - DECAY_G) < 0.01f && std::abs(b - DECAY_B) < 0.01f) {
                    decay_offset_final = offset;
                    break;
                }
            }
        }

        if (decay_offset_final) {
            offset_registry.add("Atmosphere", "Decay", *decay_offset_final);
        } else {
            LOG_ERR("Failed to find Atmosphere Decay offset");
        }

        return true;
    }

} // namespace scanner::phases