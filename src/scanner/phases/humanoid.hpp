#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "utils/offset_registry.hpp"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool humanoid(Context& ctx) {
        control::Controller controller("http://localhost:8000");

        if (!ctx.workspace.is_valid()) {
            LOG_ERR("Workspace not valid");
            return false;
        }

        auto char1 = ctx.workspace.find_first_child("Character1");
        if (!char1.is_valid()) {
            LOG_ERR("Failed to find 'Character1' in Workspace");
            return false;
        }

        auto char2 = ctx.workspace.find_first_child("Character2");
        if (!char2.is_valid()) {
            LOG_ERR("Failed to find 'Character2' in Workspace");
            return false;
        }

        auto humanoid1 = char1.find_first_child("Humanoid");
        if (!humanoid1.is_valid()) {
            LOG_ERR("Failed to find Humanoid in Character1");
            return false;
        }

        auto humanoid2 = char2.find_first_child("Humanoid");
        if (!humanoid2.is_valid()) {
            LOG_ERR("Failed to find Humanoid in Character2");
            return false;
        }

        LOG_INFO("Starting humanoid offset scan...");

        constexpr float CHAR1_HEALTH_DISPLAY_DIST = 536.0f;
        constexpr float CHAR1_NAME_DISPLAY_DIST = 204.0f;
        constexpr float CHAR1_HEALTH = 178.0f;
        constexpr float CHAR1_MAX_HEALTH = 200.0f;
        constexpr float CHAR1_HIP_HEIGHT = 92.0f;
        constexpr float CHAR1_WALK_SPEED = 298.0f;
        constexpr float CHAR1_JUMP_POWER = 91.0f;
        constexpr float CHAR1_MAX_SLOPE_ANGLE = 4.26f;
        constexpr float CHAR1_JUMP_HEIGHT = 54.457f; 

        constexpr uint8_t CHAR1_RIG_TYPE = 1;

        constexpr float CHAR2_HEALTH_DISPLAY_DIST = 201.0f;
        constexpr float CHAR2_NAME_DISPLAY_DIST = 301.0f;
        constexpr float CHAR2_HEALTH = 89.0f;
        constexpr float CHAR2_MAX_HEALTH = 187.0f;
        constexpr float CHAR2_HIP_HEIGHT = 11.0f;
        constexpr float CHAR2_WALK_SPEED = 28.0f;
        constexpr float CHAR2_JUMP_POWER = 56.0f;
        constexpr float CHAR2_MAX_SLOPE_ANGLE = 56.74f;
        constexpr float CHAR2_JUMP_HEIGHT = 78.324f; 

        constexpr uint8_t CHAR2_RIG_TYPE = 0;

        constexpr size_t SCAN_RANGE = 0x800;
        constexpr size_t ALIGNMENT = 0x4;

        auto health_display_dist_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address},
            {CHAR1_HEALTH_DISPLAY_DIST, CHAR2_HEALTH_DISPLAY_DIST}, SCAN_RANGE, ALIGNMENT);

        if (!health_display_dist_offset) {
            LOG_ERR("Failed to find HealthDisplayDistance offset");
            return false;
        }
        offset_registry.add("Humanoid", "HealthDisplayDistance", *health_display_dist_offset);

        auto name_display_dist_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address},
            {CHAR1_NAME_DISPLAY_DIST, CHAR2_NAME_DISPLAY_DIST}, SCAN_RANGE, ALIGNMENT);

        if (!name_display_dist_offset) {
            LOG_ERR("Failed to find NameDisplayDistance offset");
            return false;
        }
        offset_registry.add("Humanoid", "NameDisplayDistance", *name_display_dist_offset);

        auto health_offset =
            memory->find_verified_offset_float({humanoid1.address, humanoid2.address},
                                               {CHAR1_HEALTH, CHAR2_HEALTH}, SCAN_RANGE, ALIGNMENT);

        if (!health_offset) {
            LOG_ERR("Failed to find Health offset");
            return false;
        }
        offset_registry.add("Humanoid", "Health", *health_offset);

        auto max_health_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address}, {CHAR1_MAX_HEALTH, CHAR2_MAX_HEALTH},
            SCAN_RANGE, ALIGNMENT);

        if (!max_health_offset) {
            LOG_ERR("Failed to find MaxHealth offset");
            return false;
        }
        offset_registry.add("Humanoid", "MaxHealth", *max_health_offset);

        auto hip_height_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address}, {CHAR1_HIP_HEIGHT, CHAR2_HIP_HEIGHT},
            SCAN_RANGE, ALIGNMENT);

        if (!hip_height_offset) {
            LOG_ERR("Failed to find HipHeight offset");
            return false;
        }
        offset_registry.add("Humanoid", "HipHeight", *hip_height_offset);

        auto jump_power_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address}, {CHAR1_JUMP_POWER, CHAR2_JUMP_POWER},
            SCAN_RANGE, ALIGNMENT);

        if (!jump_power_offset) {
            LOG_ERR("Failed to find JumpPower offset");
            return false;
        }
        offset_registry.add("Humanoid", "JumpPower", *jump_power_offset);

        auto jump_height_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address}, {CHAR1_JUMP_HEIGHT, CHAR2_JUMP_HEIGHT},
            SCAN_RANGE, ALIGNMENT);

        if (!jump_height_offset) {
            LOG_ERR("Failed to find JumpHeight offset");
            return false;
        }
        offset_registry.add("Humanoid", "JumpHeight", *jump_height_offset);

        auto walkspeed_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address}, {CHAR1_WALK_SPEED, CHAR2_WALK_SPEED},
            SCAN_RANGE, ALIGNMENT);

        if (!walkspeed_offset) {
            LOG_ERR("Failed to find WalkSpeed offset");
            return false;
        }
        offset_registry.add("Humanoid", "WalkSpeed", *walkspeed_offset);

        bool found_walkspeed_check = false;
        for (size_t offset = *walkspeed_offset + ALIGNMENT; offset < SCAN_RANGE;
             offset += ALIGNMENT) {
            float value1 = memory->read<float>(humanoid1.address + offset);
            float value2 = memory->read<float>(humanoid2.address + offset);

            if (std::abs(value1 - CHAR1_WALK_SPEED) < 0.01f &&
                std::abs(value2 - CHAR2_WALK_SPEED) < 0.01f) {
                offset_registry.add("Humanoid", "WalkSpeedCheck", offset);
                found_walkspeed_check = true;
                break;
            }
        }

        if (!found_walkspeed_check) {
            LOG_ERR("Failed to find WalkSpeedCheck offset");
            return false;
        }

        auto rig_type_offset = memory->find_verified_offset<uint8_t>(
            {humanoid1.address, humanoid2.address}, {CHAR1_RIG_TYPE, CHAR2_RIG_TYPE}, SCAN_RANGE,
            0x1);

        if (!rig_type_offset) {
            LOG_ERR("Failed to find RigType offset");
            return false;
        }
        offset_registry.add("Humanoid", "RigType", *rig_type_offset);

        const auto max_slope_offset = memory->find_verified_offset_float(
            {humanoid1.address, humanoid2.address}, {CHAR1_MAX_SLOPE_ANGLE, CHAR2_MAX_SLOPE_ANGLE},
            SCAN_RANGE, ALIGNMENT);

        if (!max_slope_offset) {
            LOG_ERR("Failed to find MaxSlopeOffset offset");
            return false;
        }
        offset_registry.add("Humanoid", "MaxSlopeOffset", *max_slope_offset);

        auto npc_character = ctx.workspace.find_first_child("npc");
        if (!npc_character.is_valid()) {
            LOG_ERR("Failed to find 'npc' character in Workspace");
            return false;
        }

        auto npc_humanoid = npc_character.find_first_child("Humanoid");
        if (!npc_humanoid.is_valid()) {
            LOG_ERR("Failed to find Humanoid in NPC");
            return false;
        }

        auto head = npc_character.find_first_child("Head");
        if (!head.is_valid()) {
            LOG_ERR("Failed to find Head in NPC");
            return false;
        }

        auto head_primitive_offset = memory->find_rtti_offset(head.address, "Primitive@RBX");
        if (!head_primitive_offset) {
            LOG_ERR("Failed to find Primitive in Head");
            return false;
        }

        auto head_primitive = memory->read<uintptr_t>(head.address + *head_primitive_offset);
        if (!head_primitive) {
            LOG_ERR("Failed to read Head Primitive pointer");
            return false;
        }

        std::vector<uint8_t> walking_values = {0, 1, 0, 1};
        auto walking_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            npc_humanoid.address, walking_values,
            [&](size_t i) {
                controller.set_npc_move_to(walking_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            },
            0xA00, 0x1);

        if (walking_offsets.empty()) {
            LOG_ERR("Failed to find IsWalking offset");
            return false;
        }

        if (walking_offsets.size() > 1) {
            LOG_WARN("Found {} candidate offsets for IsWalking, using first",
                     walking_offsets.size());
        }
        offset_registry.add("Humanoid", "IsWalking", walking_offsets[0]);

        controller.set_npc_move_to(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        controller.set_npc_move_to(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        auto velocity_offset =
            memory->find_vector3_offset({head_primitive}, {{-0.011f, 0.0f, -2.0f}}, 0x600, 1.0f);

        if (!velocity_offset) {
            LOG_ERR("Failed to find AssemblyLinearVelocity offset");
            return false;
        }
        offset_registry.add("Part", "AssemblyLinearVelocity", *velocity_offset);
        offset_registry.add("Part", "AssemblyAngularVelocity", *velocity_offset + 12);

        auto walkto_offset = memory->find_vector3_offset(
            {npc_humanoid.address}, {{-41.001f, 0.5f, -728.995f}}, 0x600, 5.0f);

        if (!walkto_offset) {
            LOG_ERR("Failed to find WalkToPoint offset");
            return false;
        }
        offset_registry.add("Humanoid", "WalkToPoint", *walkto_offset);

        controller.set_npc_move_to(false);

        constexpr int PROP_SLEEP_MS = 150; 

        LOG_INFO("Scanning for AutoRotate...");
        controller.set_npc_auto_rotate(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> auto_rotate_values = {0, 1, 0, 1};
        auto auto_rotate_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            npc_humanoid.address, auto_rotate_values,
            [&](size_t i) {
                controller.set_npc_auto_rotate(auto_rotate_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            SCAN_RANGE, 0x1);

        if (auto_rotate_offsets.empty()) {
            LOG_ERR("Failed to find AutoRotate offset");
            return false;
        }
        offset_registry.add("Humanoid", "AutoRotate", auto_rotate_offsets[0]);

        LOG_INFO("Scanning for AutoJumpEnabled...");
        controller.set_npc_auto_jump_enabled(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> auto_jump_values = {1, 0, 1, 0};
        auto auto_jump_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            npc_humanoid.address, auto_jump_values,
            [&](size_t i) {
                controller.set_npc_auto_jump_enabled(auto_jump_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            SCAN_RANGE, 0x1);

        if (auto_jump_offsets.empty()) {
            LOG_ERR("Failed to find AutoJumpEnabled offset");
            return false;
        }
        offset_registry.add("Humanoid", "AutoJumpEnabled", auto_jump_offsets[0]);

        LOG_INFO("Scanning for BreakJointsOnDeath...");
        controller.set_npc_break_joints_on_death(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> break_joints_values = {1, 0, 1, 0};
        auto break_joints_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            npc_humanoid.address, break_joints_values,
            [&](size_t i) {
                controller.set_npc_break_joints_on_death(break_joints_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            SCAN_RANGE, 0x1);

        if (break_joints_offsets.empty()) {
            LOG_ERR("Failed to find BreakJointsOnDeath offset");
            return false;
        }
        offset_registry.add("Humanoid", "BreakJointsOnDeath", break_joints_offsets[0]);

        LOG_INFO("Scanning for RequiresNeck...");
        controller.set_npc_requires_neck(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> requires_neck_values = {1, 0, 1, 0};
        auto requires_neck_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            npc_humanoid.address, requires_neck_values,
            [&](size_t i) {
                controller.set_npc_requires_neck(requires_neck_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            SCAN_RANGE, 0x1);

        if (requires_neck_offsets.empty()) {
            LOG_ERR("Failed to find RequiresNeck offset");
            return false;
        }
        offset_registry.add("Humanoid", "RequiresNeck", requires_neck_offsets[0]);

        LOG_INFO("Scanning for UseJumpPower...");
        controller.set_npc_use_jump_power(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> use_jump_power_values = {1, 0, 1, 0};
        auto use_jump_power_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            npc_humanoid.address, use_jump_power_values,
            [&](size_t i) {
                controller.set_npc_use_jump_power(use_jump_power_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            SCAN_RANGE, 0x1);

        if (use_jump_power_offsets.empty()) {
            LOG_ERR("Failed to find UseJumpPower offset");
            return false;
        }
        offset_registry.add("Humanoid", "UseJumpPower", use_jump_power_offsets[0]);

        LOG_INFO("Scanning for Jump (continuous monitoring)...");

        std::vector<uint8_t> baseline(SCAN_RANGE);
        for (size_t i = 0; i < SCAN_RANGE; i++) {
            baseline[i] = memory->read<uint8_t>(npc_humanoid.address + i);
        }

        std::vector<int> toggle_count(SCAN_RANGE, 0);
        std::vector<uint8_t> last_value = baseline;

        controller.send_command("set_npc_continuous_jump", {{"enabled", true}, {"duration", 4.0f}});

        LOG_INFO("Monitoring memory for 4 seconds...");

        auto start_time = std::chrono::steady_clock::now();
        int sample_count = 0;

        while (true) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::steady_clock::now() - start_time)
                               .count();
            if (elapsed > 4000)
                break;

            for (size_t i = 0; i < SCAN_RANGE; i++) {
                uint8_t current = memory->read<uint8_t>(npc_humanoid.address + i);

                if ((last_value[i] == 0 && current == 1) || (last_value[i] == 1 && current == 0)) {
                    toggle_count[i]++;
                }

                last_value[i] = current;
            }

            sample_count++;
        }

        LOG_INFO("Took {} samples", sample_count);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::vector<std::pair<size_t, int>> candidates;
        for (size_t i = 0; i < SCAN_RANGE; i++) {
            uint8_t current = memory->read<uint8_t>(npc_humanoid.address + i);

            if (toggle_count[i] >= 4 && baseline[i] == 0 && current == 0) {
                candidates.push_back({i, toggle_count[i]});
            }
        }

        std::sort(candidates.begin(), candidates.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        LOG_INFO("Found {} candidates with 4+ toggles:", candidates.size());
        for (size_t i = 0; i < std::min(candidates.size(), (size_t)10); i++) {
            LOG_INFO("  0x{:X}: {} toggles", candidates[i].first, candidates[i].second);
        }

        if (candidates.empty()) {
            LOG_ERR("Failed to find Jump offset - no candidates");
            return false;
        }

        if (candidates.size() == 1 || candidates[0].second >= 8) {
            offset_registry.add("Humanoid", "Jump", candidates[0].first);
            LOG_INFO("Jump offset found: 0x{:X} ({} toggles)", candidates[0].first,
                     candidates[0].second);
        } else {
            offset_registry.add("Humanoid", "Jump", candidates[0].first);
            LOG_INFO("Jump offset found: 0x{:X} ({} toggles) - highest of {} candidates",
                     candidates[0].first, candidates[0].second, candidates.size());
        }

        LOG_INFO("Scanning for MoveDirection...");
        controller.set_npc_move_direction(0, 0, -1);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        auto move_direction_offset = memory->find_vector3_offset(
            {npc_humanoid.address}, {{0.0f, 0.0f, -1.0f}}, SCAN_RANGE, 0.3f);

        if (!move_direction_offset) {
            LOG_ERR("Failed to find MoveDirection offset");
            return false;
        }
        offset_registry.add("Humanoid", "MoveDirection", *move_direction_offset);

        controller.set_npc_move_direction(0, 0, 0);

        LOG_INFO("Scanning for FloorMaterial...");
        controller.move_npc_to_floor("PlasticFloor");
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

        std::vector<uint32_t> floor_values = {256, 512, 256, 512};
        auto floor_offsets = memory->find_offsets_with_snapshots<uint32_t>(
            npc_humanoid.address, floor_values,
            [&](size_t i) {
                if (floor_values[i] == 256) {
                    controller.move_npc_to_floor("PlasticFloor");
                } else {
                    controller.move_npc_to_floor("WoodFloor");
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
            },
            SCAN_RANGE, 0x4);

        if (floor_offsets.empty()) {
            LOG_ERR("Failed to find FloorMaterial offset");
            return false;
        }
        offset_registry.add("Humanoid", "FloorMaterial", floor_offsets[0]);

        LOG_INFO("Humanoid offset scan complete!");
        return true;
    }

} // namespace scanner::phases
