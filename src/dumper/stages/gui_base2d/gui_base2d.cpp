#include "gui_base2d.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::gui_base2d {

    auto dump() -> bool {
        const auto frame_absolutes = control::client::g_client.get_frame_absolutes();
        if (!frame_absolutes) {
            spdlog::error("Failed to get frame absolutes from control server");
            return false;
        }

        if (frame_absolutes->frames.empty()) {
            spdlog::error("No frames found");
            return false;
        }

        const auto replicated_storage = dumper::g_data_model.find_first_child("ReplicatedStorage");
        if (!replicated_storage->is_valid()) {
            spdlog::error("Failed to find ReplicatedStorage");
            return false;
        }

        const auto test_frames_gui = replicated_storage->find_first_child("TestFramesGui");
        if (!test_frames_gui->is_valid()) {
            spdlog::error("Failed to find TestFramesGui");
            return false;
        }

        const auto frames_folder = test_frames_gui->find_first_child("Frames");
        if (!frames_folder->is_valid()) {
            spdlog::error("Failed to find Frames folder");
            return false;
        }

        std::vector<uintptr_t> frame_addrs;
        for (const auto& frame_prop : frame_absolutes->frames) {
            const auto frame = frames_folder->find_first_child(frame_prop.name);
            if (!frame->is_valid()) {
                spdlog::error("Failed to find frame: {}", frame_prop.name);
                return false;
            }
            frame_addrs.push_back(frame->get_address());
        }

        const auto abs_rotation_offset = process::helpers::find_offset_with_getter<float>(
            frame_addrs, [&](size_t i) { return frame_absolutes->frames[i].absolute_rotation; },
            0x400, 0x4);
        if (!abs_rotation_offset) {
            spdlog::error("Failed to find AbsoluteRotation offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiBase2D", "AbsoluteRotation", *abs_rotation_offset);

        const auto abs_size_offset = process::helpers::find_offset<float>(
            frame_addrs[0], frame_absolutes->frames[0].absolute_size_x, 0x400, 0x4);
        if (!abs_size_offset) {
            spdlog::error("Failed to find AbsoluteSize offset");
            return false;
        }
        dumper::g_dumper.add_offset("GuiBase2D", "AbsoluteSize", *abs_size_offset);
        dumper::g_dumper.add_offset("GuiBase2D", "AbsolutePosition", *abs_size_offset - 0xC);

        return true;
    }

} // namespace dumper::stages::gui_base2d