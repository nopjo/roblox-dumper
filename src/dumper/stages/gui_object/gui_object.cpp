#include "gui_object.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::gui_object {

    struct FrameData {
        std::string name;
        uintptr_t address;
        control::client::FrameProperty props;
    };

    static auto get_frame_data(const control::client::FramePropertiesInfo& props)
        -> std::optional<std::vector<FrameData>> {
        std::vector<FrameData> frame_data;

        const auto starter_gui = dumper::g_data_model.find_first_child("ReplicatedStorage");
        if (!starter_gui->is_valid()) {
            spdlog::error("Failed to find ReplicatedStorage");
            return std::nullopt;
        }

        const auto test_frames_gui = starter_gui->find_first_child("TestFramesGui");
        if (!test_frames_gui->is_valid()) {
            spdlog::error("Failed to find TestFramesGui in StarterGui");
            return std::nullopt;
        }

        const auto frames_folder = test_frames_gui->find_first_child("Frames");
        if (!frames_folder->is_valid()) {
            spdlog::error("Failed to find Frames folder in TestFramesGui");
            return std::nullopt;
        }

        for (const auto& prop : props.frames) {
            const auto frame = frames_folder->find_first_child(prop.name);
            if (!frame->is_valid()) {
                spdlog::error("Failed to find frame: {}", prop.name);
                return std::nullopt;
            }

            FrameData data{.name = prop.name, .address = frame->get_address(), .props = prop};

            frame_data.push_back(data);
        }

        return frame_data;
    }

    auto dump() -> bool {
        const auto frame_props = control::client::g_client.get_frame_properties();
        if (!frame_props) {
            spdlog::error("Failed to get frame properties from control server");
            return false;
        }

        if (frame_props->frames.size() < 3) {
            spdlog::error("Not enough frames found (need at least 3)");
            return false;
        }

        const auto frames = get_frame_data(*frame_props);
        if (!frames) {
            return false;
        }

        std::vector<uintptr_t> frame_addrs;
        for (const auto& f : *frames) {
            frame_addrs.push_back(f.address);
        }

        const auto active_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.active ? 1 : 0; }, 0x800, 0x1);
        if (!active_offset) {
            spdlog::error("Failed to find Active offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Active", *active_offset);

        const auto anchor_point_offset = process::helpers::find_vec_offset<glm::vec2>(
            frame_addrs[0],
            glm::vec2((*frames)[0].props.anchor_point_x, (*frames)[0].props.anchor_point_y), 0x800,
            0.01f, 0x4);
        if (!anchor_point_offset) {
            spdlog::error("Failed to find AnchorPoint offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "AnchorPoint", *anchor_point_offset);

        const auto automatic_size_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.automatic_size; }, 0x800, 0x1);
        if (!automatic_size_offset) {
            spdlog::error("Failed to find AutomaticSize offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "AutomaticSize", *automatic_size_offset);

        const auto bg_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            frame_addrs[0],
            glm::vec3((*frames)[0].props.background_color_r, (*frames)[0].props.background_color_g,
                      (*frames)[0].props.background_color_b),
            0x800, 0.01f, 0x4);
        if (!bg_color_offset) {
            spdlog::error("Failed to find BackgroundColor3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "BackgroundColor3", *bg_color_offset);

        const auto bg_transparency_offset = process::helpers::find_offset_with_getter<float>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.background_transparency; },
            0x800, 0x4);
        if (!bg_transparency_offset) {
            spdlog::error("Failed to find BackgroundTransparency offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "BackgroundTransparency", *bg_transparency_offset);

        const auto border_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            frame_addrs[0],
            glm::vec3((*frames)[0].props.border_color_r, (*frames)[0].props.border_color_g,
                      (*frames)[0].props.border_color_b),
            0x800, 0.01f, 0x4);
        if (!border_color_offset) {
            spdlog::error("Failed to find BorderColor3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "BorderColor3", *border_color_offset);

        const auto border_mode_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.border_mode; }, 0x800, 0x1);
        if (!border_mode_offset) {
            spdlog::error("Failed to find BorderMode offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "BorderMode", *border_mode_offset);

        const auto border_size_offset = process::helpers::find_offset_with_getter<int32_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.border_size_pixel; }, 0x800,
            0x4);
        if (!border_size_offset) {
            spdlog::error("Failed to find BorderSizePixel offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "BorderSizePixel", *border_size_offset);

        const auto clips_descendants_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.clips_descendants ? 1 : 0; },
            0x800, 0x1);
        if (!clips_descendants_offset) {
            spdlog::error("Failed to find ClipsDescendants offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "ClipsDescendants", *clips_descendants_offset);

        const auto gui_state_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.gui_state; }, 0x800, 0x1);
        if (!gui_state_offset) {
            spdlog::error("Failed to find GuiState offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "GuiState", *gui_state_offset);

        const auto interactable_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.interactable ? 1 : 0; }, 0x800,
            0x1);
        if (!interactable_offset) {
            spdlog::error("Failed to find Interactable offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Interactable", *interactable_offset);

        const auto layout_order_offset = process::helpers::find_offset_with_getter<int32_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.layout_order; }, 0x800, 0x4);
        if (!layout_order_offset) {
            spdlog::error("Failed to find LayoutOrder offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "LayoutOrder", *layout_order_offset);

        const auto position_offset = process::helpers::find_offset<float>(
            frame_addrs[0], (*frames)[0].props.position_x_scale, 0x800, 0x4);
        if (!position_offset) {
            spdlog::error("Failed to find Position offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Position", *position_offset);

        const auto rotation_offset = process::helpers::find_offset_with_getter<float>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.rotation; }, 0x800, 0x4);
        if (!rotation_offset) {
            spdlog::error("Failed to find Rotation offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Rotation", *rotation_offset);

        const auto selectable_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.selectable ? 1 : 0; }, 0x800,
            0x1);
        if (!selectable_offset) {
            spdlog::error("Failed to find Selectable offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Selectable", *selectable_offset);

        const auto selection_order_offset = process::helpers::find_offset_with_getter<int32_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.selection_order; }, 0x800, 0x4);
        if (!selection_order_offset) {
            spdlog::error("Failed to find SelectionOrder offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "SelectionOrder", *selection_order_offset);

        const auto size_offset = process::helpers::find_offset<float>(
            frame_addrs[0], (*frames)[0].props.size_x_scale, 0x800, 0x4);
        if (!size_offset) {
            spdlog::error("Failed to find Size offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Size", *size_offset);

        const auto size_constraint_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.size_constraint; }, 0x800, 0x1);
        if (!size_constraint_offset) {
            spdlog::error("Failed to find SizeConstraint offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "SizeConstraint", *size_constraint_offset);

        const auto visible_offset = process::helpers::find_offset_with_getter<uint8_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.visible ? 1 : 0; }, 0x800, 0x1);
        if (!visible_offset) {
            spdlog::error("Failed to find Visible offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "Visible", *visible_offset);

        const auto zindex_offset = process::helpers::find_offset_with_getter<int32_t>(
            frame_addrs, [&](size_t i) { return (*frames)[i].props.z_index; }, 0x800, 0x4);
        if (!zindex_offset) {
            spdlog::error("Failed to find ZIndex offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiObject", "ZIndex", *zindex_offset);

        return true;
    }

} // namespace dumper::stages::gui_object