#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace control::client {
    using json = nlohmann::json;

    struct DataModelInfo {
        uint64_t place_id;
        uint64_t game_id;
        uint64_t creator_id;
        std::string job_id;
    };

    struct WorkspaceInfo {
        uint32_t children_count;
        float gravity;
    };

    struct PlayerInfo {
        uint64_t user_id;
        uint32_t account_age;
        std::string display_name;
        std::string locale_id;
    };

    struct PartProperty {
        std::string name;
        bool anchored;
        bool can_collide;
        bool can_touch;
        bool cast_shadow;
        bool locked;
        bool massless;

        float pos_x, pos_y, pos_z;
        float rotation_x, rotation_y, rotation_z;
        float size_x, size_y, size_z;
        float transparency;
        float reflectance;

        uint8_t color_r, color_g, color_b;

        uint16_t material;
        uint8_t shape;
    };

    struct PartPropertiesInfo {
        std::vector<PartProperty> parts;
    };

    struct HumanoidProperty {
        std::string name;
        float health;
        float max_health;
        float walk_speed;
        float jump_power;
        float jump_height;
        float hip_height;
        float max_slope_angle;
        float health_display_distance;
        float name_display_distance;

        bool auto_jump_enabled;
        bool automatic_scaling_enabled;
        bool auto_rotate;
        bool break_joints_on_death;
        bool evaluate_state_machine;
        bool jump;
        bool requires_neck;
        bool sit;
        bool use_jump_power;

        float camera_offset_x, camera_offset_y, camera_offset_z;
        float target_point_x, target_point_y, target_point_z;
        float walk_to_point_x, walk_to_point_y, walk_to_point_z;

        uint8_t rig_type;
        uint8_t display_distance_type;
        uint8_t health_display_type;
        uint8_t name_occlusion;
    };

    struct HumanoidPropertiesInfo {
        std::vector<HumanoidProperty> humanoids;
    };

    struct ViewportInfo {
        float viewport_width;
        float viewport_height;
    };

    struct FrameProperty {
        std::string name;
        bool active;
        float anchor_point_x, anchor_point_y;
        uint8_t automatic_size;
        float background_color_r, background_color_g, background_color_b;
        float background_transparency;
        float border_color_r, border_color_g, border_color_b;
        uint8_t border_mode;
        int32_t border_size_pixel;
        bool clips_descendants;
        uint8_t gui_state;
        bool interactable;
        int32_t layout_order;
        float position_x_scale, position_x_offset, position_y_scale, position_y_offset;
        float rotation;
        bool selectable;
        int32_t selection_order;
        float size_x_scale, size_x_offset, size_y_scale, size_y_offset;
        uint8_t size_constraint;
        bool visible;
        int32_t z_index;
    };

    struct FramePropertiesInfo {
        std::vector<FrameProperty> frames;
    };

    struct FrameAbsoluteProperty {
        std::string name;
        float absolute_rotation;
        float absolute_size_x, absolute_size_y;
    };

    struct FrameAbsolutesInfo {
        std::vector<FrameAbsoluteProperty> frames;
    };

    struct CharacterMeshProperty {
        std::string name;
        uint8_t body_part;
        uint64_t base_texture_id;
        uint64_t mesh_id;
        uint64_t overlay_texture_id;
    };

    struct CharacterMeshPropertiesInfo {
        std::vector<CharacterMeshProperty> meshes;
    };

    struct ToolProperty {
        std::string name;
        bool can_be_dropped;
        bool enabled;
        bool manual_activation_only;
        bool requires_handle;
        float grip_pos_x, grip_pos_y, grip_pos_z;
        float grip_forward_x, grip_forward_y, grip_forward_z;
        float grip_right_x, grip_right_y, grip_right_z;
        float grip_up_x, grip_up_y, grip_up_z;
        std::string tool_tip;
    };

    struct ToolPropertiesInfo {
        std::vector<ToolProperty> tools;
    };

    struct ProximityPromptProperty {
        std::string name;
        std::string action_text;
        std::string object_text;
        bool auto_localize;
        bool clickable_prompt;
        bool enabled;
        bool requires_line_of_sight;
        uint8_t exclusivity;
        uint32_t gamepad_key_code;
        uint32_t keyboard_key_code;
        float hold_duration;
        float max_activation_distance;
        float max_indicator_distance;
        uint8_t style;
        float ui_offset_x;
        float ui_offset_y;
    };

    struct ProximityPromptPropertiesInfo {
        std::vector<ProximityPromptProperty> prompts;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DataModelInfo, place_id, game_id, creator_id, job_id)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WorkspaceInfo, children_count, gravity)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerInfo, user_id, account_age, display_name, locale_id)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PartProperty, name, anchored, can_collide, can_touch,
                                       cast_shadow, locked, massless, shape, pos_x, pos_y, pos_z,
                                       rotation_x, rotation_y, rotation_z, size_x, size_y, size_z,
                                       color_r, color_g, color_b, transparency, reflectance,
                                       material)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PartPropertiesInfo, parts)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        HumanoidProperty, name, health, max_health, walk_speed, jump_power, jump_height, hip_height,
        max_slope_angle, health_display_distance, name_display_distance, auto_jump_enabled,
        automatic_scaling_enabled, auto_rotate, break_joints_on_death, evaluate_state_machine, jump,
        requires_neck, sit, use_jump_power, camera_offset_x, camera_offset_y, camera_offset_z,
        target_point_x, target_point_y, target_point_z, walk_to_point_x, walk_to_point_y,
        walk_to_point_z, rig_type, display_distance_type, health_display_type, name_occlusion)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HumanoidPropertiesInfo, humanoids)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ViewportInfo, viewport_width, viewport_height)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FrameProperty, name, active, anchor_point_x, anchor_point_y,
                                       automatic_size, background_color_r, background_color_g,
                                       background_color_b, background_transparency, border_color_r,
                                       border_color_g, border_color_b, border_mode,
                                       border_size_pixel, clips_descendants, gui_state,
                                       interactable, layout_order, position_x_scale,
                                       position_x_offset, position_y_scale, position_y_offset,
                                       rotation, selectable, selection_order, size_x_scale,
                                       size_x_offset, size_y_scale, size_y_offset, size_constraint,
                                       visible, z_index)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FramePropertiesInfo, frames)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FrameAbsoluteProperty, name, absolute_rotation,
                                       absolute_size_x, absolute_size_y)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FrameAbsolutesInfo, frames)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CharacterMeshProperty, name, body_part, base_texture_id,
                                       mesh_id, overlay_texture_id)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CharacterMeshPropertiesInfo, meshes)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ToolProperty, name, can_be_dropped, enabled,
                                       manual_activation_only, requires_handle, grip_pos_x,
                                       grip_pos_y, grip_pos_z, grip_forward_x, grip_forward_y,
                                       grip_forward_z, grip_right_x, grip_right_y, grip_right_z,
                                       grip_up_x, grip_up_y, grip_up_z, tool_tip)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ToolPropertiesInfo, tools)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProximityPromptProperty, name, action_text, object_text,
                                       auto_localize, clickable_prompt, enabled,
                                       requires_line_of_sight, exclusivity, gamepad_key_code,
                                       keyboard_key_code, hold_duration, max_activation_distance,
                                       max_indicator_distance, style, ui_offset_x, ui_offset_y)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProximityPromptPropertiesInfo, prompts)

} // namespace control::client