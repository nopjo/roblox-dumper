#pragma once
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool ui_gradient(Context& ctx) {
        const auto replicated_storage =
            sdk::instance_t(ctx.data_model).find_first_child("ReplicatedStorage");
        if (!replicated_storage.is_valid()) {
            LOG_ERR("Failed to find ReplicatedStorage");
            return false;
        }

        const auto ui_gradient = replicated_storage.find_first_child_of_class("UIGradient");
        if (!ui_gradient.is_valid()) {
            LOG_ERR("Failed to find UIGradient in 'ReplicatedStorage'");
            return false;
        }

        constexpr float COLOR_R = 156.0f / 255.0f;
        constexpr float COLOR_G = 89.0f / 255.0f;
        constexpr float COLOR_B = 182.0f / 255.0f;

        auto color_offset =
            memory->find_verified_offset_float({ui_gradient.address}, {COLOR_R}, 0x600, 0x4);

        if (color_offset) {
            float g = memory->read<float>(ui_gradient.address + *color_offset + 4);
            float b = memory->read<float>(ui_gradient.address + *color_offset + 8);
            if (std::abs(g - COLOR_G) < 0.01f && std::abs(b - COLOR_B) < 0.01f) {
                offset_registry.add("UIGradient", "Color", *color_offset);
            } else {
                LOG_ERR("Failed to verify Color G/B channels for UIGradient");
            }
        } else {
            LOG_ERR("Failed to find Color offset for UIGradient");
        }

        const auto offset_offset =
            memory->find_verified_offset_float({ui_gradient.address}, {102.45f}, 0x400, 0x2);

        if (!offset_offset) {
            LOG_ERR("Failed to find offset offset for UIGradient");
            return false;
        }

        offset_registry.add("UIGradient", "Offset", *offset_offset);

        const auto rotation_offset =
            memory->find_verified_offset_float({ui_gradient.address}, {67.67f}, 0x400, 0x2);

        if (!rotation_offset) {
            LOG_ERR("Failed to find Rotation offset for UIGradient");
            return false;
        }

        offset_registry.add("UIGradient", "Rotation", *rotation_offset);

        const auto transparency_offset =
            memory->find_verified_offset_float({ui_gradient.address}, {42.56f}, 0x400, 0x2);

        if (!transparency_offset) {
            LOG_ERR("Failed to find Transparency offset for UIGradient");
            return false;
        }

        offset_registry.add("UIGradient", "Transparency", *transparency_offset);

        return true;
    }

} // namespace scanner::phases