#include "tool.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::tool {

    struct ToolData {
        std::string name;
        uintptr_t address;
        control::client::ToolProperty props;
    };

    static auto get_tool_data(const control::client::ToolPropertiesInfo& props)
        -> std::optional<std::vector<ToolData>> {
        std::vector<ToolData> tool_data;

        auto tools_folder = dumper::g_workspace->find_first_child("Tools");
        if (!tools_folder->is_valid()) {
            spdlog::error("Failed to find Tools folder");
            return std::nullopt;
        }

        for (const auto& prop : props.tools) {
            const auto tool = tools_folder->find_first_child(prop.name);
            if (!tool->is_valid()) {
                spdlog::error("Failed to find tool: {}", prop.name);
                return std::nullopt;
            }

            ToolData data{.name = prop.name, .address = tool->get_address(), .props = prop};

            tool_data.push_back(data);
        }

        return tool_data;
    }

    auto dump() -> bool {
        const auto tool_props = control::client::g_client.get_tool_properties();
        if (!tool_props) {
            spdlog::error("Failed to get tool properties from control server");
            return false;
        }

        if (tool_props->tools.size() < 3) {
            spdlog::error("Not enough tools found (need at least 3)");
            return false;
        }

        const auto tools = get_tool_data(*tool_props);
        if (!tools) {
            return false;
        }

        std::vector<uintptr_t> tool_addrs;
        for (const auto& t : *tools) {
            tool_addrs.push_back(t.address);
        }

        const auto can_be_dropped_offset = process::helpers::find_offset_with_getter<uint8_t>(
            tool_addrs, [&](size_t i) { return (*tools)[i].props.can_be_dropped ? 1 : 0; }, 0x600,
            0x1);
        if (!can_be_dropped_offset) {
            spdlog::error("Failed to find CanBeDropped offset");
            return false;
        }
        dumper::g_dumper.add_offset("Tool", "CanBeDropped", *can_be_dropped_offset);

        const auto enabled_offset = process::helpers::find_offset_with_getter<uint8_t>(
            tool_addrs, [&](size_t i) { return (*tools)[i].props.enabled ? 1 : 0; }, 0x600, 0x1);
        if (!enabled_offset) {
            spdlog::error("Failed to find Enabled offset");
            return false;
        }
        dumper::g_dumper.add_offset("Tool", "Enabled", *enabled_offset);

        const auto manual_activation_offset = process::helpers::find_offset_with_getter<uint8_t>(
            tool_addrs, [&](size_t i) { return (*tools)[i].props.manual_activation_only ? 1 : 0; },
            0x600, 0x1);
        if (!manual_activation_offset) {
            spdlog::error("Failed to find ManualActivationOnly offset");
            return false;
        }
        dumper::g_dumper.add_offset("Tool", "ManualActivationOnly", *manual_activation_offset);

        const auto requires_handle_offset = process::helpers::find_offset_with_getter<uint8_t>(
            tool_addrs, [&](size_t i) { return (*tools)[i].props.requires_handle ? 1 : 0; }, 0x600,
            0x1);
        if (!requires_handle_offset) {
            spdlog::error("Failed to find RequiresHandle offset");
            return false;
        }
        dumper::g_dumper.add_offset("Tool", "RequiresHandle", *requires_handle_offset);

        const auto grip_pos_offset =
            process::helpers::find_vec3_offset_multi<glm::vec3>(tool_addrs, [&](size_t i) {
                const auto& t = (*tools)[i].props;
                return glm::vec3(t.grip_pos_x, t.grip_pos_y, t.grip_pos_z);
            });
        if (!grip_pos_offset) {
            spdlog::error("Failed to find GripPos offset");
            return false;
        }
        dumper::g_dumper.add_offset("Tool", "GripPos", *grip_pos_offset);
        dumper::g_dumper.add_offset("Tool", "Grip", *grip_pos_offset - 0x24);
        dumper::g_dumper.add_offset("Tool", "GripRight", *grip_pos_offset - 0x24);
        dumper::g_dumper.add_offset("Tool", "GripUp", *grip_pos_offset - 0x18);
        dumper::g_dumper.add_offset("Tool", "GripForward", *grip_pos_offset - 0xC);

        if (!tools->empty()) {
            const auto& first_tool = (*tools)[0];
            const auto tooltip_offset = process::helpers::find_string_offset(
                first_tool.address, first_tool.props.tool_tip, 0x800, 0x8, 0x256, true);
            if (!tooltip_offset) {
                spdlog::error("Failed to get Tooltip offset for Tool");
                return false;
            }

            else {
                dumper::g_dumper.add_offset("Tool", "Tooltip", *tooltip_offset);
            }
        }

        return true;
    }

} // namespace dumper::stages::tool