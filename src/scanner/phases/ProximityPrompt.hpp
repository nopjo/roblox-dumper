#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool proximity_prompt(Context& ctx) {
        const auto replicated_storage =
            sdk::instance_t(ctx.data_model).find_first_child("ReplicatedStorage");
        if (!replicated_storage.is_valid()) {
            LOG_ERR("Failed to find ReplicatedStorage");
            return false;
        }

        const auto test_part = replicated_storage.find_first_child("ProximityPromptTest");
        if (!test_part.is_valid()) {
            LOG_ERR("Failed to find ProximityPromptTest in ReplicatedStorage");
            return false;
        }

        const auto prompt = test_part.find_first_child("TestPrompt");
        if (!prompt.is_valid()) {
            LOG_ERR("Failed to find TestPrompt");
            return false;
        }

        control::Controller controller("http://localhost:8000");
        constexpr int SLEEP_MS = 200;

        const auto action_text_offset =
            memory->find_roblox_string_direct(prompt.address, "TestAction123", 0x400, 0x8);
        if (!action_text_offset) {
            LOG_ERR("Failed to find ActionText offset");
            return false;
        }
        offset_registry.add("ProximityPrompt", "ActionText", *action_text_offset);
        LOG_SUCCESS("ActionText: 0x{:x}", *action_text_offset);

        const auto object_text_offset =
            memory->find_roblox_string_direct(prompt.address, "TestObject456", 0x400, 0x8);
        if (!object_text_offset) {
            LOG_ERR("Failed to find ObjectText offset");
            return false;
        }
        offset_registry.add("ProximityPrompt", "ObjectText", *object_text_offset);
        LOG_SUCCESS("ObjectText: 0x{:x}", *object_text_offset);

        const auto hold_duration_offset =
            memory->find_verified_offset_float({prompt.address}, {2.5f}, 0x400, 0x4);
        if (!hold_duration_offset) {
            LOG_ERR("Failed to find HoldDuration offset");
            return false;
        }
        offset_registry.add("ProximityPrompt", "HoldDuration", *hold_duration_offset);
        LOG_SUCCESS("HoldDuration: 0x{:x}", *hold_duration_offset);

        const auto keyboard_keycode_offset = memory->find_value_offset<int32_t>(prompt.address, 101, 0x400, 0x4);
        if (keyboard_keycode_offset) {
            offset_registry.add("ProximityPrompt", "KeyboardKeyCode", *keyboard_keycode_offset);
            LOG_SUCCESS("KeyboardKeyCode: 0x{:x}", *keyboard_keycode_offset);
        } else {
            LOG_WARN("KeyboardKeyCode search failed");
        }

        offset_registry.add("ProximityPrompt", "KeyboardKeyCode", *keyboard_keycode_offset);
        LOG_SUCCESS("KeyboardKeyCode: 0x{:x}", *keyboard_keycode_offset);

        std::vector<float> max_distance_values = {12.5f, 20.0f, 12.5f};

        controller.set_proximity_prompt_max_distance(max_distance_values[0]);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

        auto max_distance_offsets = memory->find_offsets_with_snapshots<float>(
            prompt.address, max_distance_values,
            [&](size_t i) {
                controller.set_proximity_prompt_max_distance(max_distance_values[i]);
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            },
            0x400, 0x4, SLEEP_MS);

        if (!max_distance_offsets.empty()) {
            offset_registry.add("ProximityPrompt", "MaxActivationDistance",
                                max_distance_offsets[0]);
            LOG_SUCCESS("MaxActivationDistance: 0x{:x}", max_distance_offsets[0]);
        } else {
            LOG_ERR("Failed to find MaxActivationDistance offset");
        }

        std::vector<uint8_t> enabled_values = {1, 0, 1};

        controller.set_proximity_prompt_enabled(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

        auto enabled_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            prompt.address, enabled_values,
            [&](size_t i) {
                controller.set_proximity_prompt_enabled(enabled_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            },
            0x400, 0x1, SLEEP_MS);

        if (!enabled_offsets.empty()) {
            offset_registry.add("ProximityPrompt", "Enabled", enabled_offsets[0]);
            LOG_SUCCESS("Enabled: 0x{:x}", enabled_offsets[0]);
        } else {
            LOG_ERR("Failed to find Enabled offset");
        }

        std::vector<uint8_t> requires_los_values = {0, 1, 0};

        controller.set_proximity_prompt_requires_line_of_sight(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

        auto requires_los_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            prompt.address, requires_los_values,
            [&](size_t i) {
                controller.set_proximity_prompt_requires_line_of_sight(requires_los_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            },
            0x400, 0x1, SLEEP_MS);

        if (!requires_los_offsets.empty()) {
            offset_registry.add("ProximityPrompt", "RequiresLineOfSight", requires_los_offsets[0]);
            LOG_SUCCESS("RequiresLineOfSight: 0x{:x}", requires_los_offsets[0]);
        } else {
            LOG_ERR("Failed to find RequiresLineOfSight offset");
        }

        LOG_INFO("ProximityPrompt phase completed successfully!");
        return true;
    }

} // namespace scanner::phases
