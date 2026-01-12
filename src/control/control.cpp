#include "control.hpp"
#include "utils/logger.hpp"
#include <cpr/cpr.h>

namespace control {
    Controller::Controller(const std::string& api_url) : api_url(api_url) {}

    std::string Controller::send_command(const std::string& action, const nlohmann::json& data) {
        nlohmann::json payload = {{"action", action}, {"data", data}};

        LOG_INFO("Sending command: {} with data: {}", action, data.dump());

        auto response =
            cpr::Post(cpr::Url{api_url + "/execute"},
                      cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{payload.dump()});

        if (response.status_code == 200) {
            auto json_response = nlohmann::json::parse(response.text);
            std::string cmd_id = json_response["command_id"];
            return cmd_id;
        }

        LOG_ERR("Failed to send command, status code: {}", response.status_code);
        return "";
    }

    std::optional<CommandResult> Controller::get_status(const std::string& command_id) {
        auto response = cpr::Get(cpr::Url{api_url + "/status/" + command_id});

        if (response.status_code == 200) {
            auto json_response = nlohmann::json::parse(response.text);

            if (json_response["status"] != "not_found") {
                CommandResult result;
                result.status = json_response["status"];
                result.result = json_response["result"];
                return result;
            }
        }

        return std::nullopt;
    }

    std::optional<CommandResult> Controller::wait_for_completion(const std::string& command_id,
                                                                 int timeout_ms,
                                                                 int poll_interval_ms) {
        auto start = std::chrono::steady_clock::now();

        while (true) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::steady_clock::now() - start)
                               .count();

            if (elapsed >= timeout_ms) {
                LOG_ERR("Command {} timed out after {}ms", command_id, timeout_ms);
                return std::nullopt;
            }

            auto status = get_status(command_id);
            if (status.has_value() && status->status != "pending") {
                return status;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_ms));
        }
    }

    bool Controller::execute_command(const std::string& action, const nlohmann::json& data) {
        std::string command_id = send_command(action, data);
        if (command_id.empty())
            return false;
        auto result = wait_for_completion(command_id);
        return result.has_value() && result->status == "completed";
    }

    bool Controller::set_gravity(float value) {
        return execute_command("set_gravity", {{"value", value}});
    }

    bool Controller::set_skybox_orientation(float x, float y, float z) {
        return execute_command("set_skybox_orientation", {{"x", x}, {"y", y}, {"z", z}});
    }

    bool Controller::set_npc_move_to(bool enabled) {
        return execute_command("set_npc_move_to", {{"enabled", enabled}});
    }

    bool Controller::set_camera_fov(float value) {
        return execute_command("set_camera_fov", {{"value", value}});
    }

    bool Controller::set_camera_position(float x, float y, float z) {
        return execute_command("set_camera_position", {{"x", x}, {"y", y}, {"z", z}});
    }

    bool Controller::set_camera_rotation(float x, float y, float z) {
        return execute_command("set_camera_rotation", {{"x", x}, {"y", y}, {"z", z}});
    }

    bool Controller::set_team(const std::string& team) {
        return execute_command("set_team", {{"team", team}});
    }

    bool Controller::set_frame_position(float x_scale, int x_offset, float y_scale, int y_offset) {
        return execute_command("set_frame_position", {{"x_scale", x_scale},
                                                      {"x_offset", x_offset},
                                                      {"y_scale", y_scale},
                                                      {"y_offset", y_offset}});
    }

    bool Controller::set_frame_size(float x_scale, int x_offset, float y_scale, int y_offset) {
        return execute_command("set_frame_size", {{"x_scale", x_scale},
                                                  {"x_offset", x_offset},
                                                  {"y_scale", y_scale},
                                                  {"y_offset", y_offset}});
    }

    bool Controller::set_frame_visible(bool value) {
        return execute_command("set_frame_visible", {{"value", value}});
    }

    bool Controller::set_tool_can_be_dropped(bool value) {
        return execute_command("set_tool_can_be_dropped", {{"value", value}});
    }

    bool Controller::set_tool_enabled(bool value) {
        return execute_command("set_tool_enabled", {{"value", value}});
    }

    bool Controller::set_tool_manual_activation(bool value) {
        return execute_command("set_tool_manual_activation", {{"value", value}});
    }

    bool Controller::set_tool_requires_handle(bool value) {
        return execute_command("set_tool_requires_handle", {{"value", value}});
    }

    bool Controller::set_tool_grip_pos(float x, float y, float z) {
        return execute_command("set_tool_grip_pos", {{"x", x}, {"y", y}, {"z", z}});
    }

    bool Controller::set_highlight_depth_mode(int value) {
        return execute_command("set_highlight_depth_mode", {{"value", value}});
    }

    bool Controller::set_proximity_prompt_enabled(bool value) {
        return execute_command("set_proximity_prompt_enabled", {{"value", value}});
    }

    bool Controller::set_proximity_prompt_requires_line_of_sight(bool value) {
        return execute_command("set_proximity_prompt_requires_line_of_sight", {{"value", value}});
    }

    bool Controller::set_npc_auto_rotate(bool value) {
        return execute_command("set_npc_auto_rotate", {{"value", value}});
    }

    bool Controller::set_npc_auto_jump_enabled(bool value) {
        return execute_command("set_npc_auto_jump_enabled", {{"value", value}});
    }

    bool Controller::set_npc_break_joints_on_death(bool value) {
        return execute_command("set_npc_break_joints_on_death", {{"value", value}});
    }

    bool Controller::set_npc_requires_neck(bool value) {
        return execute_command("set_npc_requires_neck", {{"value", value}});
    }

    bool Controller::set_npc_use_jump_power(bool value) {
        return execute_command("set_npc_use_jump_power", {{"value", value}});
    }

    bool Controller::set_npc_continuous_jump(bool enabled, float duration) {
        return execute_command("set_npc_continuous_jump",
                               {{"enabled", enabled}, {"duration", duration}});
    }

    bool Controller::set_npc_move_direction(float x, float y, float z) {
        return execute_command("set_npc_move_direction", {{"x", x}, {"y", y}, {"z", z}});
    }

    bool Controller::move_npc_to_floor(const std::string& floor_name) {
        return execute_command("move_npc_to_floor", {{"floor_name", floor_name}});
    }

    bool Controller::set_part_cast_shadow(const std::string& part_name, bool value) {
        return execute_command("set_part_cast_shadow",
                               {{"part_name", part_name}, {"value", value}});
    }

    bool Controller::set_part_locked(const std::string& part_name, bool value) {
        return execute_command("set_part_locked", {{"part_name", part_name}, {"value", value}});
    }

    bool Controller::set_part_massless(const std::string& part_name, bool value) {
        return execute_command("set_part_massless", {{"part_name", part_name}, {"value", value}});
    }

    bool Controller::request_client_gui_info(const std::string& frame_name) {
        return execute_command("request_client_gui_info", {{"frame_name", frame_name}});
    }

    std::optional<Controller::GuiAbsoluteInfo> Controller::get_client_gui_info() {
        std::string cmd_id = send_command("get_client_gui_info", {});
        if (cmd_id.empty())
            return std::nullopt;

        auto result = wait_for_completion(cmd_id, 3000);
        if (!result.has_value() || result->status != "completed") {
            return std::nullopt;
        }

        try {
            GuiAbsoluteInfo info;
            info.abs_pos_x = result->result["abs_pos_x"].get<float>();
            info.abs_pos_y = result->result["abs_pos_y"].get<float>();
            info.abs_size_x = result->result["abs_size_x"].get<float>();
            info.abs_size_y = result->result["abs_size_y"].get<float>();
            return info;
        } catch (...) {
            return std::nullopt;
        }
    }

    void Controller::set_api_url(const std::string& url) { this->api_url = url; }

    bool Controller::clear_queue() {
        auto response = cpr::Post(cpr::Url{api_url + "/clear"});
        return response.status_code == 200;
    }
} // namespace control
