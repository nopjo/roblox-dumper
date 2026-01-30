#include "mouse_service.h"
#include "dumper/dumper.h"
#include "process/memory/memory.h"
#include "process/rtti/rtti.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::mouse_service {

    auto dump() -> bool {
        const auto mouse_service = dumper::g_data_model.find_first_child_of_class("MouseService");

        if (!mouse_service) {
            spdlog::error("Failed to find MouseService in DataModel");
            return false;
        }

        const auto input_objects =
            process::Rtti::find_all(mouse_service->get_address(), "InputObject@RBX", 0x400, 0x8);

        if (input_objects.size() < 2) {
            spdlog::error("Failed to find second occurrence of InputObject in MouseService");
            return false;
        }

        const auto second_input_object = input_objects[1];
        auto input_object_ptr =
            process::Memory::read<uintptr_t>(mouse_service->get_address() + second_input_object);

        if (!input_object_ptr) {
            spdlog::error("Failed to read InputObject pointer");
            return false;
        }

        auto mouse_pos = process::Memory::read<glm::vec2>(*input_object_ptr + 0xEC);
        if (!mouse_pos) {
            spdlog::error("Failed to read mouse position");
            return false;
        }

        if (mouse_pos->x < 0 || mouse_pos->x > 10000 || mouse_pos->y < 0 || mouse_pos->y > 10000) {
            spdlog::error("Invalid mouse position: ({}, {})", mouse_pos->x, mouse_pos->y);
            return false;
        }

        dumper::g_dumper.add_offset("MouseService", "InputObject", second_input_object);
        dumper::g_dumper.add_offset("InputObject", "MousePosition", 0xEC);

        return true;
    }
} // namespace dumper::stages::mouse_service