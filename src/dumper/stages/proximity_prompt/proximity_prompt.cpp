#include "proximity_prompt.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::proximity_prompt {

    struct ProximityPromptData {
        std::string name;
        uintptr_t address;
        control::client::ProximityPromptProperty props;
    };

    static auto
    get_proximity_prompt_data(const control::client::ProximityPromptPropertiesInfo& props)
        -> std::optional<std::vector<ProximityPromptData>> {
        std::vector<ProximityPromptData> prompt_data;

        auto prompts_folder = dumper::g_workspace->find_first_child("ProximityPrompts");
        if (!prompts_folder->is_valid()) {
            spdlog::error("Failed to find ProximityPrompts folder");
            return std::nullopt;
        }

        for (const auto& prop : props.prompts) {
            auto part_name = "PromptPart" + prop.name.substr(10);
            auto part = prompts_folder->find_first_child(part_name);
            if (!part->is_valid()) {
                spdlog::error("Failed to find part: {}", part_name);
                return std::nullopt;
            }

            auto prompt = part->find_first_child(prop.name);
            if (!prompt->is_valid()) {
                spdlog::error("Failed to find proximity prompt: {}", prop.name);
                return std::nullopt;
            }

            ProximityPromptData data{
                .name = prop.name, .address = prompt->get_address(), .props = prop};

            prompt_data.push_back(data);
        }

        return prompt_data;
    }

    auto dump() -> bool {
        const auto prompt_props = control::client::g_client.get_proximity_prompt_properties();
        if (!prompt_props) {
            spdlog::error("Failed to get proximity prompt properties from control server");
            return false;
        }

        if (prompt_props->prompts.size() < 3) {
            spdlog::error("Not enough proximity prompts found (need at least 3)");
            return false;
        }

        const auto prompts = get_proximity_prompt_data(*prompt_props);
        if (!prompts) {
            return false;
        }

        std::vector<uintptr_t> prompt_addrs;
        for (const auto& p : *prompts) {
            prompt_addrs.push_back(p.address);
        }

        const auto enabled_offset = process::helpers::find_offset_with_getter<uint8_t>(
            prompt_addrs, [&](size_t i) { return (*prompts)[i].props.enabled ? 1 : 0; }, 0x300,
            0x1);
        if (!enabled_offset) {
            spdlog::error("Failed to find Enabled offset");
            return false;
        }
        dumper::g_dumper.add_offset("ProximityPrompt", "Enabled", *enabled_offset);

        const auto requires_los_offset = process::helpers::find_offset_with_getter<uint8_t>(
            prompt_addrs,
            [&](size_t i) { return (*prompts)[i].props.requires_line_of_sight ? 1 : 0; }, 0x300,
            0x1);
        if (!requires_los_offset) {
            spdlog::error("Failed to find RequiresLineOfSight offset");
            return false;
        }
        dumper::g_dumper.add_offset("ProximityPrompt", "RequiresLineOfSight", *requires_los_offset);

        const auto hold_duration_offset = process::helpers::find_offset_with_getter<float>(
            prompt_addrs, [&](size_t i) { return (*prompts)[i].props.hold_duration; }, 0x300, 0x4);
        if (!hold_duration_offset) {
            spdlog::error("Failed to find HoldDuration offset");
            return false;
        }
        dumper::g_dumper.add_offset("ProximityPrompt", "HoldDuration", *hold_duration_offset);

        const auto keyboard_key_offset = process::helpers::find_offset_with_getter<uint32_t>(
            prompt_addrs, [&](size_t i) { return (*prompts)[i].props.keyboard_key_code; }, 0x300,
            0x4);
        if (!keyboard_key_offset) {
            spdlog::error("Failed to find KeyboardKeyCode offset");
            return false;
        }
        dumper::g_dumper.add_offset("ProximityPrompt", "KeyboardKeyCode", *keyboard_key_offset);

        const auto max_activation_offset = process::helpers::find_offset_with_getter<float>(
            prompt_addrs, [&](size_t i) { return (*prompts)[i].props.max_activation_distance; },
            0x300, 0x4);
        if (!max_activation_offset) {
            spdlog::error("Failed to find MaxActivationDistance offset");
            return false;
        }
        dumper::g_dumper.add_offset("ProximityPrompt", "MaxActivationDistance",
                                    *max_activation_offset);

        if (!prompts->empty()) {
            const auto& first_prompt = (*prompts)[0];

            const auto action_text_offset = process::helpers::find_string_offset(
                first_prompt.address, first_prompt.props.action_text, 0x300, 0x8, 0x256, true);
            if (action_text_offset) {
                dumper::g_dumper.add_offset("ProximityPrompt", "ActionText", *action_text_offset);
            } else {
                spdlog::error("Failed to find ActionText offset");
                return false;
            }

            const auto object_text_offset = process::helpers::find_string_offset(
                first_prompt.address, first_prompt.props.object_text, 0x300, 0x8, 0x256, true);
            if (object_text_offset) {
                dumper::g_dumper.add_offset("ProximityPrompt", "ObjectText", *object_text_offset);
            } else {
                spdlog::error("Failed to find ObjectText offset");
                return false;
            }
        }

        return true;
    }

} // namespace dumper::stages::proximity_prompt