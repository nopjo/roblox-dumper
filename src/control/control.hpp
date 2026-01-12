#pragma once
#include <chrono>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <thread>

namespace control {
    struct CommandResult {
        std::string status;
        nlohmann::json result;
    };

    class Controller {
      public:
        Controller(const std::string& api_url = "http://localhost:8000");

        std::string send_command(const std::string& action, const nlohmann::json& data);
        std::optional<CommandResult> get_status(const std::string& command_id);
        std::optional<CommandResult> wait_for_completion(const std::string& command_id,
                                                         int timeout_ms = 5000,
                                                         int poll_interval_ms = 50);

        bool set_gravity(float value);
        bool set_skybox_orientation(float x, float y, float z);
        bool set_npc_move_to(bool enabled);
        bool set_camera_fov(float value);
        bool set_camera_position(float x, float y, float z);
        bool set_camera_rotation(float x, float y, float z);
        bool set_team(const std::string& team);
        bool set_frame_position(float x_scale, int x_offset, float y_scale, int y_offset);
        bool set_frame_size(float x_scale, int x_offset, float y_scale, int y_offset);
        bool set_frame_visible(bool value);
        bool set_tool_can_be_dropped(bool value);
        bool set_tool_enabled(bool value);
        bool set_tool_manual_activation(bool value);
        bool set_tool_requires_handle(bool value);
        bool set_tool_grip_pos(float x, float y, float z);
        bool set_highlight_depth_mode(int value);
        bool set_proximity_prompt_enabled(bool value);
        bool set_proximity_prompt_requires_line_of_sight(bool value);
        bool set_npc_auto_rotate(bool value);
        bool set_npc_auto_jump_enabled(bool value);
        bool set_npc_break_joints_on_death(bool value);
        bool set_npc_requires_neck(bool value);
        bool set_npc_use_jump_power(bool value);
        bool set_npc_continuous_jump(bool enabled, float duration = 3.0f);
        bool set_npc_move_direction(float x, float y, float z);
        bool move_npc_to_floor(const std::string& floor_name);
        bool set_part_cast_shadow(const std::string& part_name, bool value);
        bool set_part_locked(const std::string& part_name, bool value);
        bool set_part_massless(const std::string& part_name, bool value);

        struct GuiAbsoluteInfo {
            float abs_pos_x;
            float abs_pos_y;
            float abs_size_x;
            float abs_size_y;
        };

        std::optional<GuiAbsoluteInfo> get_client_gui_info();
        bool request_client_gui_info(const std::string& frame_name = "hello");

        void set_api_url(const std::string& url);
        bool clear_queue();

      private:
        std::string api_url;
        bool execute_command(const std::string& action, const nlohmann::json& data);
    };
} // namespace control
