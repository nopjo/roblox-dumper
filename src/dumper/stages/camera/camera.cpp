#include "camera.h"
#include "control/client/client.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

auto dumper::stages::camera::dump() -> bool {
    const auto camera = dumper::g_workspace->find_first_child_of_class("Camera");

    if (!camera) {
        spdlog::error("Failed to find Camera in Workspace");
        return false;
    }

    auto viewport_info = control::client::g_client.get_viewport_size();
    if (!viewport_info) {
        spdlog::error("Failed to get viewport size from client");
        return false;
    }

    float fov_radians = glm::radians(113.2f);
    FIND_AND_ADD_OFFSET(camera->get_address(), Camera, float, FieldOfView, fov_radians, 0x400, 0x4);

    glm::vec3 camera_position(45.2f, 19.4f, 50.0f);
    const auto position_offset =
        process::helpers::find_vec_offset(camera->get_address(), camera_position, 0x400, 5.0f, 0x4);

    if (!position_offset) {
        spdlog::error("Failed to find Position in Camera");
        return false;
    }

    g_dumper.add_offset("Camera", "Position", *position_offset);
    g_dumper.add_offset("Camera", "Rotation", *position_offset - 0x24);
    g_dumper.add_offset("Camera", "CFrame", *position_offset - 0x24);

    glm::vec2 viewport_size(viewport_info->viewport_width, viewport_info->viewport_height);
    const auto viewport_offset =
        process::helpers::find_vec_offset(camera->get_address(), viewport_size, 0x400, 5.0f, 0x4);

    if (!viewport_offset) {
        spdlog::error("Failed to find ViewportSize in Camera");
        return false;
    }

    g_dumper.add_offset("Camera", "ViewportSize", *viewport_offset);

    auto viewport_x_int = static_cast<int16_t>(viewport_info->viewport_width);
    auto viewport_y_int = static_cast<int16_t>(viewport_info->viewport_height);

    for (size_t offset = 0; offset < 0x400; offset += 0x2) {
        auto x = process::Memory::read<int16_t>(camera->get_address() + offset);
        auto y = process::Memory::read<int16_t>(camera->get_address() + offset + 0x2);

        if (x && y && *x == viewport_x_int && *y == viewport_y_int) {
            g_dumper.add_offset("Camera", "ViewportInt16", offset);
            break;
        }
    }

    return true;
}