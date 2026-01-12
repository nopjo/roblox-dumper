#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool gui_object(Context& ctx) {
        const auto replicated_storage =
            sdk::instance_t(ctx.data_model).find_first_child("ReplicatedStorage");
        if (!replicated_storage.is_valid()) {
            LOG_ERR("Failed to find ReplicatedStorage");
            return false;
        }

        const auto screen_gui = replicated_storage.find_first_child_of_class("ScreenGui");
        if (!screen_gui.is_valid()) {
            LOG_ERR("Failed to find ScreenGui in 'ReplicatedStorage'");
            return false;
        }

        const auto frame = screen_gui.find_first_child_of_class("Frame");
        if (!frame.is_valid()) {
            LOG_ERR("Failed to find Frame in 'ReplicatedStorage.ScreenGui'");
            return false;
        }

        const auto anchor_point_offset =
            memory->find_verified_offset_float({frame.address}, {0.69f}, 0x800, 0x2);

        if (!anchor_point_offset) {
            LOG_ERR("Failed to find AnchorPoint offset for GuiObject");
            return false;
        }

        offset_registry.add("GuiObject", "AnchorPoint", *anchor_point_offset);

        const auto bg_transparency_offset =
            memory->find_verified_offset_float({frame.address}, {0.139f}, 0x800, 0x2);

        if (!bg_transparency_offset) {
            LOG_ERR("Failed to find BackgroundTransparency offset for GuiObject");
            return false;
        }

        offset_registry.add("GuiObject", "BackgroundTransparency", *bg_transparency_offset);

        const auto border_size_pixel_offset =
            memory->find_value_offset<uint32_t>(frame.address, 42, 0x800, 0x4);

        if (!border_size_pixel_offset) {
            LOG_ERR("Failed to find BorderSizePixel offset for GuiObject");
            return false;
        }

        offset_registry.add("GuiObject", "BorderSizePixel", *border_size_pixel_offset);

        const auto layout_order_offset =
            memory->find_value_offset<uint32_t>(frame.address, 107, 0x800, 0x4);

        if (!layout_order_offset) {
            LOG_ERR("Failed to find LayoutOrder offset for GuiObject");
            return false;
        }

        offset_registry.add("GuiObject", "LayoutOrder", *layout_order_offset);

        const auto rotation_offset =
            memory->find_verified_offset_float({frame.address}, {85.2f}, 0x800, 0x2);

        if (!rotation_offset) {
            LOG_ERR("Failed to find Rotation offset for GuiObject");
            return false;
        }

        offset_registry.add("GuiObject", "Rotation", *rotation_offset);

        const auto zindex_offset =
            memory->find_value_offset<uint32_t>(frame.address, 39, 0x800, 0x4);

        if (!zindex_offset) {
            LOG_ERR("Failed to find ZIndex offset for GuiObject");
            return false;
        }

        offset_registry.add("GuiObject", "ZIndex", *zindex_offset);

        constexpr float BG_COLOR_R = 231.0f / 255.0f;
        constexpr float BG_COLOR_G = 76.0f / 255.0f;
        constexpr float BG_COLOR_B = 60.0f / 255.0f;

        auto bg_color_offset =
            memory->find_verified_offset_float({frame.address}, {BG_COLOR_R}, 0x800, 0x4);

        if (bg_color_offset) {
            float g = memory->read<float>(frame.address + *bg_color_offset + 4);
            float b = memory->read<float>(frame.address + *bg_color_offset + 8);
            if (std::abs(g - BG_COLOR_G) < 0.01f && std::abs(b - BG_COLOR_B) < 0.01f) {
                offset_registry.add("GuiObject", "BackgroundColor3", *bg_color_offset);
            } else {
                LOG_ERR("Failed to verify BackgroundColor3 G/B channels for GuiObject");
            }
        } else {
            LOG_ERR("Failed to find BackgroundColor3 offset for GuiObject");
        }

        constexpr float BORDER_COLOR_R = 52.0f / 255.0f;
        constexpr float BORDER_COLOR_G = 152.0f / 255.0f;
        constexpr float BORDER_COLOR_B = 219.0f / 255.0f;

        auto border_color_offset =
            memory->find_verified_offset_float({frame.address}, {BORDER_COLOR_R}, 0x800, 0x4);

        if (border_color_offset) {
            float g = memory->read<float>(frame.address + *border_color_offset + 4);
            float b = memory->read<float>(frame.address + *border_color_offset + 8);
            if (std::abs(g - BORDER_COLOR_G) < 0.01f && std::abs(b - BORDER_COLOR_B) < 0.01f) {
                offset_registry.add("GuiObject", "BorderColor3", *border_color_offset);
            } else {
                LOG_ERR("Failed to verify BorderColor3 G/B channels for GuiObject");
            }
        } else {
            LOG_ERR("Failed to find BorderColor3 offset for GuiObject");
        }

        control::Controller controller("http://localhost:8000");

        {
            std::vector<UDim2> positions = {
                {{0.1f, 10}, {0.2f, 20}},
                {{0.5f, 50}, {0.6f, 60}},
                {{0.3f, 30}, {0.4f, 40}},
            };

            controller.set_frame_position(positions[0].X.Scale, positions[0].X.Offset,
                                          positions[0].Y.Scale, positions[0].Y.Offset);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            auto offsets = memory->find_offsets_with_snapshots<UDim2>(
                frame.address, positions,
                [&](size_t i) {
                    controller.set_frame_position(positions[i].X.Scale, positions[i].X.Offset,
                                                  positions[i].Y.Scale, positions[i].Y.Offset);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                },
                0x800, 0x4);

            if (!offsets.empty()) {
                offset_registry.add("GuiObject", "Position", offsets[0]);
            } else {
                LOG_ERR("Failed to find Position offset for GuiObject");
            }
        }

        {
            std::vector<UDim2> sizes = {
                {{0.25f, 100}, {0.35f, 150}},
                {{0.75f, 300}, {0.85f, 400}},
                {{0.45f, 200}, {0.55f, 250}},
            };

            controller.set_frame_size(sizes[0].X.Scale, sizes[0].X.Offset, sizes[0].Y.Scale,
                                      sizes[0].Y.Offset);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            auto offsets = memory->find_offsets_with_snapshots<UDim2>(
                frame.address, sizes,
                [&](size_t i) {
                    controller.set_frame_size(sizes[i].X.Scale, sizes[i].X.Offset, sizes[i].Y.Scale,
                                              sizes[i].Y.Offset);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                },
                0x800, 0x4);

            if (!offsets.empty()) {
                offset_registry.add("GuiObject", "Size", offsets[0]);
            } else {
                LOG_ERR("Failed to find Size offset for GuiObject");
            }
        }


        LOG_INFO("Scanning for Visible...");

        constexpr int PROP_SLEEP_MS = 200;

        controller.set_frame_visible(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> visible_values = {1, 0, 1, 0};
        auto visible_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            frame.address, visible_values,
            [&](size_t i) {
                controller.set_frame_visible(visible_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            0x800, 0x1);

        if (visible_offsets.empty()) {
            LOG_ERR("Failed to find Visible offset for GuiObject");
            return false;
        }
        offset_registry.add("GuiObject", "Visible", visible_offsets[0]);


       LOG_INFO("Scanning for AbsolutePosition/AbsoluteSize...");

        const auto starter_gui = sdk::instance_t(ctx.data_model).find_first_child("StarterGui");
        if (!starter_gui.is_valid()) {
            LOG_ERR("Failed to find StarterGui");
            return false;
        }

        const auto gui_screen_gui = starter_gui.find_first_child("ScreenGui");
        if (!gui_screen_gui.is_valid()) {
            LOG_ERR("Failed to find ScreenGui in StarterGui");
            return false;
        }

        const auto hello_frame = gui_screen_gui.find_first_child("hello");
        if (!hello_frame.is_valid()) {
            LOG_ERR("Failed to find 'hello' frame in ScreenGui");
            return false;
        }

        LOG_INFO("Found 'hello' frame at address: 0x{:X}", hello_frame.address);

        controller.request_client_gui_info("hello");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        auto abs_info = controller.get_client_gui_info();
        if (!abs_info) {
            LOG_ERR("Failed to get AbsolutePosition/Size from client");
            return false;
        }

        LOG_INFO("Client reported: AbsPosX={}, AbsSizeX={}", abs_info->abs_pos_x,
                 abs_info->abs_size_x);

        constexpr size_t GUI_SCAN_RANGE = 0x4000;
        constexpr float TOLERANCE = 1.0f;

        std::optional<size_t> abs_pos_x_offset;
        for (size_t offset = 0; offset < GUI_SCAN_RANGE; offset += 4) {
            float value = memory->read<float>(hello_frame.address + offset);
            if (std::abs(value - abs_info->abs_pos_x) < TOLERANCE) {
                abs_pos_x_offset = offset;
                float y_value = memory->read<float>(hello_frame.address + offset + 4);
                LOG_INFO("Found AbsolutePosition at 0x{:X} (X={}, Y={})", offset, value, y_value);
                break;
            }
        }

        std::optional<size_t> abs_size_x_offset;
        for (size_t offset = 0; offset < GUI_SCAN_RANGE; offset += 4) {
            float value = memory->read<float>(hello_frame.address + offset);
            if (std::abs(value - abs_info->abs_size_x) < TOLERANCE) {
                abs_size_x_offset = offset;
                float y_value = memory->read<float>(hello_frame.address + offset + 4);
                LOG_INFO("Found AbsoluteSize at 0x{:X} (X={}, Y={})", offset, value, y_value);
                break;
            }
        }

        if (!abs_pos_x_offset) {
            LOG_ERR("Failed to find AbsolutePositionX offset");
            return false;
        }
        offset_registry.add("GuiObject", "AbsolutePositionX", *abs_pos_x_offset);
        offset_registry.add("GuiObject", "AbsolutePositionY", *abs_pos_x_offset + 4);

        if (!abs_size_x_offset) {
            LOG_ERR("Failed to find AbsoluteSizeX offset");
            return false;
        }
        offset_registry.add("GuiObject", "AbsoluteSizeX", *abs_size_x_offset);
        offset_registry.add("GuiObject", "AbsoluteSizeY", *abs_size_x_offset + 4);

        LOG_INFO("AbsolutePosition offset: 0x{:X}", *abs_pos_x_offset);
        LOG_INFO("AbsoluteSize offset: 0x{:X}", *abs_size_x_offset);

        return true;
    }


} // namespace scanner::phases
