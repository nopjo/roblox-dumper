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

        return true;
    }

} // namespace scanner::phases