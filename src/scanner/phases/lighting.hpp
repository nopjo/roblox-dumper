#pragma once
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool lighting(Context& ctx) {
        auto lighting = sdk::instance_t(ctx.data_model).find_first_child("Lighting");
        if (!lighting.is_valid()) {
            LOG_ERR("Failed to find Lighting");
            return false;
        }

        control::Controller controller("http://localhost:8000");
        constexpr int SLEEP_MS = 500;

        auto sky = memory->find_rtti_offset(lighting.address, "Sky@RBX");
        if (!sky) {
            LOG_ERR("Failed to get Sky pointer inside Lighting");
            return false;
        }

        offset_registry.add("Lighting", "Sky", *sky);

        auto atmosphere = memory->find_rtti_offset(lighting.address, "Atmosphere@RBX");
        if (!atmosphere) {
            LOG_ERR("Failed to get Atmosphere pointer inside Lighting");
            return false;
        }

        offset_registry.add("Lighting", "Atmosphere", *atmosphere);

        const auto brightness_offset =
            memory->find_verified_offset_float({lighting.address}, {3.456f}, 0x400, 0x2);

        if (!brightness_offset) {
            LOG_ERR("Failed to find Brightness offset");
            return false;
        }

        offset_registry.add("Lighting", "Brightness", *brightness_offset);

        // note for future me if i forget. clock time is stored in microseconds
        const auto clock_time =
            memory->find_value_offset<uint64_t>(lighting.address, 21600000000, 0x400, 0x4); // 6am

        if (!clock_time) {
            LOG_ERR("Failed to find ClockTime offset");
            return false;
        }

        offset_registry.add("Lighting", "ClockTime", *clock_time);

        const auto environment_diffuse_scale_offset =
            memory->find_verified_offset_float({lighting.address}, {0.817f}, 0x400, 0x4);

        if (!environment_diffuse_scale_offset) {
            LOG_ERR("Failed to find EnvironmentDiffuseScale offset");
            return false;
        }

        offset_registry.add("Lighting", "EnvironmentDiffuseScale",
                            *environment_diffuse_scale_offset);

        const auto environment_specular_scale_offset =
            memory->find_verified_offset_float({lighting.address}, {0.521f}, 0x400, 0x4);

        if (!environment_specular_scale_offset) {
            LOG_ERR("Failed to find EnvironmentSpecularScale offset");
            return false;
        }

        offset_registry.add("Lighting", "EnvironmentSpecularScale",
                            *environment_specular_scale_offset);

        constexpr float AMBIENT_R = 142.0f / 255.0f;
        constexpr float AMBIENT_G = 68.0f / 255.0f;
        constexpr float AMBIENT_B = 173.0f / 255.0f;

        auto ambient_offset =
            memory->find_verified_offset_float({lighting.address}, {AMBIENT_R}, 0x600, 0x4);

        if (ambient_offset) {
            float g = memory->read<float>(lighting.address + *ambient_offset + 4);
            float b = memory->read<float>(lighting.address + *ambient_offset + 8);
            if (std::abs(g - AMBIENT_G) < 0.01f && std::abs(b - AMBIENT_B) < 0.01f) {
                offset_registry.add("Lighting", "Ambient", *ambient_offset);
            } else {
                LOG_ERR("Failed to verify Ambient G/B channels");
            }
        } else {
            LOG_ERR("Failed to find Ambient offset");
        }

        constexpr float OUTDOOR_R = 125.0f / 255.0f;
        constexpr float OUTDOOR_G = 206.0f / 255.0f;
        constexpr float OUTDOOR_B = 160.0f / 255.0f;

        auto outdoor_offset =
            memory->find_verified_offset_float({lighting.address}, {OUTDOOR_R}, 0x600, 0x4);

        if (outdoor_offset) {
            float g = memory->read<float>(lighting.address + *outdoor_offset + 4);
            float b = memory->read<float>(lighting.address + *outdoor_offset + 8);
            if (std::abs(g - OUTDOOR_G) < 0.01f && std::abs(b - OUTDOOR_B) < 0.01f) {
                offset_registry.add("Lighting", "OutdoorAmbient", *outdoor_offset);
            } else {
                LOG_ERR("Failed to verify OutdoorAmbient G/B channels");
            }
        } else {
            LOG_ERR("Failed to find OutdoorAmbient offset");
        }

        constexpr float SHIFT_TOP_R = 99.0f / 255.0f;
        constexpr float SHIFT_TOP_G = 110.0f / 255.0f;
        constexpr float SHIFT_TOP_B = 250.0f / 255.0f;

        auto shift_top_offset =
            memory->find_verified_offset_float({lighting.address}, {SHIFT_TOP_R}, 0x600, 0x4);

        if (shift_top_offset) {
            float g = memory->read<float>(lighting.address + *shift_top_offset + 4);
            float b = memory->read<float>(lighting.address + *shift_top_offset + 8);
            if (std::abs(g - SHIFT_TOP_G) < 0.01f && std::abs(b - SHIFT_TOP_B) < 0.01f) {
                offset_registry.add("Lighting", "ColorShift_Top", *shift_top_offset);
            } else {
                LOG_ERR("Failed to verify ColorShift_Top G/B channels");
            }
        } else {
            LOG_ERR("Failed to find ColorShift_Top offset");
        }

        constexpr float SHIFT_BOTTOM_R = 85.0f / 255.0f;
        constexpr float SHIFT_BOTTOM_G = 239.0f / 255.0f;
        constexpr float SHIFT_BOTTOM_B = 196.0f / 255.0f;

        auto shift_bottom_offset =
            memory->find_verified_offset_float({lighting.address}, {SHIFT_BOTTOM_R}, 0x600, 0x4);

        if (shift_bottom_offset) {
            float g = memory->read<float>(lighting.address + *shift_bottom_offset + 4);
            float b = memory->read<float>(lighting.address + *shift_bottom_offset + 8);
            if (std::abs(g - SHIFT_BOTTOM_G) < 0.01f && std::abs(b - SHIFT_BOTTOM_B) < 0.01f) {
                offset_registry.add("Lighting", "ColorShift_Bottom", *shift_bottom_offset);
            } else {
                LOG_ERR("Failed to verify ColorShift_Bottom G/B channels");
            }
        } else {
            LOG_ERR("Failed to find ColorShift_Bottom offset");
        }

        return true;
    }

} // namespace scanner::phases