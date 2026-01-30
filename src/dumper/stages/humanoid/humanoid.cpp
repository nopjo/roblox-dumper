#include "humanoid.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <thread>

namespace dumper::stages::humanoid {

    struct HumanoidData {
        std::string name;
        uintptr_t address;
        control::client::HumanoidProperty props;
    };

    static auto get_humanoid_data(const control::client::HumanoidPropertiesInfo& props)
        -> std::optional<std::vector<HumanoidData>> {
        std::vector<HumanoidData> humanoid_data;

        auto characters_folder = dumper::g_workspace->find_first_child("Characters");
        if (!characters_folder->is_valid()) {
            spdlog::error("Failed to find Characters folder");
            return std::nullopt;
        }

        for (const auto& prop : props.humanoids) {
            auto character = characters_folder->find_first_child(prop.name);
            if (!character->is_valid()) {
                spdlog::error("Failed to find character: {}", prop.name);
                return std::nullopt;
            }

            auto humanoid = character->find_first_child("Humanoid");
            if (!humanoid->is_valid()) {
                spdlog::error("Failed to find Humanoid in {}", prop.name);
                return std::nullopt;
            }

            HumanoidData data{.name = prop.name, .address = humanoid->get_address(), .props = prop};

            humanoid_data.push_back(data);
        }

        return humanoid_data;
    }

    auto dump() -> bool {
        const auto humanoid_props = control::client::g_client.get_humanoid_properties();
        if (!humanoid_props) {
            spdlog::error("Failed to get humanoid properties from control server");
            return false;
        }

        if (humanoid_props->humanoids.size() < 3) {
            spdlog::error("Not enough humanoids found (need at least 3)");
            return false;
        }

        const auto humanoids = get_humanoid_data(*humanoid_props);
        if (!humanoids) {
            return false;
        }

        std::vector<uintptr_t> humanoid_addrs;
        for (const auto& h : *humanoids) {
            humanoid_addrs.push_back(h.address);
        }

        const auto health_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.health; }, 0x800, 0x4);
        if (!health_offset) {
            spdlog::error("Failed to find Health offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "Health", *health_offset);

        const auto max_health_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.max_health; }, 0x800, 0x4);
        if (!max_health_offset) {
            spdlog::error("Failed to find MaxHealth offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "MaxHealth", *max_health_offset);

        const auto walk_speed_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.walk_speed; }, 0x800, 0x4);
        if (!walk_speed_offset) {
            spdlog::error("Failed to find WalkSpeed offset");
            return false;
        }

        const auto walk_speed_check_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.walk_speed; }, 0x800, 0x4,
            {*walk_speed_offset});

        if (walk_speed_check_offset) {
            dumper::g_dumper.add_offset("Humanoid", "WalkSpeed", *walk_speed_offset);
            dumper::g_dumper.add_offset("Humanoid", "WalkSpeedCheck", *walk_speed_check_offset);
        }

        const auto jump_power_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.jump_power; }, 0x800, 0x4);
        if (!jump_power_offset) {
            spdlog::error("Failed to find JumpPower offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "JumpPower", *jump_power_offset);

        const auto jump_height_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.jump_height; }, 0x800,
            0x4);
        if (!jump_height_offset) {
            spdlog::error("Failed to find JumpHeight offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "JumpHeight", *jump_height_offset);

        const auto hip_height_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.hip_height; }, 0x800, 0x4);
        if (!hip_height_offset) {
            spdlog::error("Failed to find HipHeight offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "HipHeight", *hip_height_offset);

        const auto max_slope_angle_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.max_slope_angle; }, 0x800,
            0x4);
        if (!max_slope_angle_offset) {
            spdlog::error("Failed to find MaxSlopeAngle offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "MaxSlopeAngle", *max_slope_angle_offset);

        const auto health_display_distance_offset =
            process::helpers::find_offset_with_getter<float>(
                humanoid_addrs,
                [&](size_t i) { return (*humanoids)[i].props.health_display_distance; }, 0x800,
                0x4);
        if (!health_display_distance_offset) {
            spdlog::error("Failed to find HealthDisplayDistance offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "HealthDisplayDistance",
                                    *health_display_distance_offset);

        const auto name_display_distance_offset = process::helpers::find_offset_with_getter<float>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.name_display_distance; },
            0x800, 0x4);
        if (!name_display_distance_offset) {
            spdlog::error("Failed to find NameDisplayDistance offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "NameDisplayDistance",
                                    *name_display_distance_offset);

        const auto auto_jump_enabled_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs,
            [&](size_t i) { return (*humanoids)[i].props.auto_jump_enabled ? 1 : 0; }, 0x800, 0x1);
        if (!auto_jump_enabled_offset) {
            spdlog::error("Failed to find AutoJumpEnabled offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "AutoJumpEnabled", *auto_jump_enabled_offset);

        const auto automatic_scaling_enabled_offset =
            process::helpers::find_offset_with_getter<uint8_t>(
                humanoid_addrs,
                [&](size_t i) { return (*humanoids)[i].props.automatic_scaling_enabled ? 1 : 0; },
                0x800, 0x1);
        if (!automatic_scaling_enabled_offset) {
            spdlog::error("Failed to find AutomaticScalingEnabled offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "AutomaticScalingEnabled",
                                    *automatic_scaling_enabled_offset);

        const auto auto_rotate_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.auto_rotate ? 1 : 0; },
            0x800, 0x1);
        if (!auto_rotate_offset) {
            spdlog::error("Failed to find AutoRotate offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "AutoRotate", *auto_rotate_offset);

        const auto break_joints_on_death_offset =
            process::helpers::find_offset_with_getter<uint8_t>(
                humanoid_addrs,
                [&](size_t i) { return (*humanoids)[i].props.break_joints_on_death ? 1 : 0; },
                0x800, 0x1);
        if (!break_joints_on_death_offset) {
            spdlog::error("Failed to find BreakJointsOnDeath offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "BreakJointsOnDeath",
                                    *break_joints_on_death_offset);

        const auto evaluate_state_machine_offset =
            process::helpers::find_offset_with_getter<uint8_t>(
                humanoid_addrs,
                [&](size_t i) { return (*humanoids)[i].props.evaluate_state_machine ? 1 : 0; },
                0x800, 0x1);
        if (!evaluate_state_machine_offset) {
            spdlog::error("Failed to find EvaluateStateMachine offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "EvaluateStateMachine",
                                    *evaluate_state_machine_offset);

        const auto requires_neck_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.requires_neck ? 1 : 0; },
            0x800, 0x1);
        if (!requires_neck_offset) {
            spdlog::error("Failed to find RequiresNeck offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "RequiresNeck", *requires_neck_offset);

        const auto sit_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.sit ? 1 : 0; }, 0x800,
            0x1);
        if (!sit_offset) {
            spdlog::error("Failed to find Sit offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "Sit", *sit_offset);

        const auto use_jump_power_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.use_jump_power ? 1 : 0; },
            0x800, 0x1);
        if (!use_jump_power_offset) {
            spdlog::error("Failed to find UseJumpPower offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "UseJumpPower", *use_jump_power_offset);

        const auto camera_offset_offset = process::helpers::find_vec3_offset_multi<glm::vec3>(
            humanoid_addrs,
            [&](size_t i) {
                const auto& p = (*humanoids)[i].props;
                return glm::vec3(p.camera_offset_x, p.camera_offset_y, p.camera_offset_z);
            },
            0x800, 0.01f);
        if (!camera_offset_offset) {
            spdlog::error("Failed to find CameraOffset offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "CameraOffset", *camera_offset_offset);

        const auto target_point_offset = process::helpers::find_vec3_offset_multi<glm::vec3>(
            humanoid_addrs,
            [&](size_t i) {
                const auto& p = (*humanoids)[i].props;
                return glm::vec3(p.target_point_x, p.target_point_y, p.target_point_z);
            },
            0x800, 5.0f);
        if (!target_point_offset) {
            spdlog::error("Failed to find TargetPoint offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "TargetPoint", *target_point_offset);

        const auto walk_to_point_offset = process::helpers::find_vec3_offset_multi<glm::vec3>(
            humanoid_addrs,
            [&](size_t i) {
                const auto& p = (*humanoids)[i].props;
                return glm::vec3(p.walk_to_point_x, p.walk_to_point_y, p.walk_to_point_z);
            },
            0x800, 5.0f);
        if (!walk_to_point_offset) {
            spdlog::error("Failed to find WalkToPoint offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "WalkToPoint", *walk_to_point_offset);

        const auto rig_type_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.rig_type; }, 0x800, 0x1);
        if (!rig_type_offset) {
            spdlog::error("Failed to find RigType offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "RigType", *rig_type_offset);

        const auto display_distance_type_offset =
            process::helpers::find_offset_with_getter<uint8_t>(
                humanoid_addrs,
                [&](size_t i) { return (*humanoids)[i].props.display_distance_type; }, 0x800, 0x1);
        if (!display_distance_type_offset) {
            spdlog::error("Failed to find DisplayDistanceType offset");
            return false;
        }

        dumper::g_dumper.add_offset("Humanoid", "DisplayDistanceType",
                                    *display_distance_type_offset);

        const auto health_display_type_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.health_display_type; },
            0x800, 0x1);
        if (!health_display_type_offset) {
            spdlog::error("Failed to find HealthDisplayType offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "HealthDisplayType", *health_display_type_offset);

        const auto name_occlusion_offset = process::helpers::find_offset_with_getter<uint8_t>(
            humanoid_addrs, [&](size_t i) { return (*humanoids)[i].props.name_occlusion; }, 0x800,
            0x1);
        if (!name_occlusion_offset) {
            spdlog::error("Failed to find NameOcclusion offset");
            return false;
        }
        dumper::g_dumper.add_offset("Humanoid", "NameOcclusion", *name_occlusion_offset);

        return true;
    }

} // namespace dumper::stages::humanoid