#pragma once
#include "roblox/instance/instance.h"
#include "types.h"
#include <optional>

namespace bridge {

    class Bridge {
      public:
        Bridge() = default;

        auto initialize() -> bool;

        auto read_game_information() -> std::optional<DataModelInfo>;
        auto read_player_information() -> std::optional<PlayerInfo>;
        auto read_parts_information() -> std::optional<PartPropertiesInfo>;
        auto read_humanoid_information() -> std::optional<HumanoidPropertiesInfo>;
        auto read_camera_information() -> std::optional<ViewportInfo>;
        auto read_character_mesh_information() -> std::optional<CharacterMeshPropertiesInfo>;
        auto read_frames_information() -> std::optional<FramePropertiesInfo>;
        auto read_frame_absolutes_information() -> std::optional<FrameAbsolutesInfo>;
        auto read_tools_information() -> std::optional<ToolPropertiesInfo>;
        auto read_proximity_prompts_information() -> std::optional<ProximityPromptPropertiesInfo>;
        auto read_text_labels_information() -> std::optional<TextLabelPropertiesInfo>;
        auto read_text_buttons_information() -> std::optional<TextButtonPropertiesInfo>;

      private:
        auto read_value(const roblox::Instance& instance) -> std::optional<json>;

        template <typename T>
        auto read_as(const roblox::Instance& instance, const char* name) -> std::optional<T>;

        std::optional<roblox::Instance> m_game_information;
        std::optional<roblox::Instance> m_player_information;
        std::optional<roblox::Instance> m_parts_information;
        std::optional<roblox::Instance> m_humanoid_information;
        std::optional<roblox::Instance> m_camera_information;
        std::optional<roblox::Instance> m_character_mesh_information;
        std::optional<roblox::Instance> m_frames_information;
        std::optional<roblox::Instance> m_frame_absolutes_information;
        std::optional<roblox::Instance> m_tools_information;
        std::optional<roblox::Instance> m_proximity_prompts_information;
        std::optional<roblox::Instance> m_text_labels_information;
        std::optional<roblox::Instance> m_text_buttons_information;
    };

    inline Bridge g_bridge;

} // namespace bridge
