#pragma once
#include "types.h"
#include <optional>
#include <string>

namespace control::client {

    class Client {
      public:
        Client(const std::string& base_url = "http://localhost:8080");

        auto get_data_model_information() -> std::optional<DataModelInfo>;
        auto get_workspace_information() -> std::optional<WorkspaceInfo>;
        auto get_player_information() -> std::optional<PlayerInfo>;
        auto get_part_properties() -> std::optional<PartPropertiesInfo>;
        auto set_part_properties() -> std::optional<PartPropertiesInfo>;
        auto get_humanoid_properties() -> std::optional<HumanoidPropertiesInfo>;
        auto get_viewport_size() -> std::optional<ViewportInfo>;
        auto get_frame_properties() -> std::optional<FramePropertiesInfo>;
        auto get_frame_absolutes() -> std::optional<FrameAbsolutesInfo>;
        auto get_character_mesh_properties() -> std::optional<CharacterMeshPropertiesInfo>;
        auto get_tool_properties() -> std::optional<ToolPropertiesInfo>;
        auto get_proximity_prompt_properties() -> std::optional<ProximityPromptPropertiesInfo>;

      private:
        template <typename T> auto execute(const std::string& action) -> std::optional<T>;

        auto send_request(const std::string& endpoint, const json& payload = {})
            -> std::optional<json>;

        std::string m_base_url;
    };

    inline Client g_client;

} // namespace control::client