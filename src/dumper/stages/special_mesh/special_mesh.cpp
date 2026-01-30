#include "special_mesh.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::special_mesh {

    auto dump() -> bool {
        const auto special_mesh = dumper::g_workspace->find_first_child("SpecialMeshPart")
                                      ->find_first_child_of_class("SpecialMesh");

        if (!special_mesh) {
            spdlog::error("Could not find Special Mesh");
            return false;
        }

        const auto mesh_id = process::helpers::find_sso_string_offset(
            special_mesh->get_address(), "http://www.roblox.com/Asset/?id=9982590", 0x800, 0x8,
            true);

        if (!mesh_id) {
            spdlog::error("Failed to get MeshId for SpecialMesh");
        }

        dumper::g_dumper.add_offset("SpecialMesh", "MeshId", *mesh_id);

        const auto texture_id = process::helpers::find_sso_string_offset(
            special_mesh->get_address(), "rbxassetid://9982590", 0x800, 0x8, true);

        if (!texture_id) {
            spdlog::error("Failed to get TextureId for SpecialMesh");
        }

        dumper::g_dumper.add_offset("SpecialMesh", "TextureId", *texture_id);

        glm::vec3 offset(1.4f, 10.5f, 11.2f);

        const auto mesh_offset = process::helpers::find_vec_offset<glm::vec3>(
            special_mesh->get_address(), offset, 0x400, 0.01f, 0x4);

        if (mesh_offset) {
            g_dumper.add_offset("SpecialMesh", "Offset", *mesh_offset);
        } else {
            spdlog::error("Failed to find Offset in SpecialMesh");
        }

        glm::vec3 scale(2.2f, 4.4f, 1.2f);

        const auto scale_offset = process::helpers::find_vec_offset<glm::vec3>(
            special_mesh->get_address(), scale, 0x400, 0.01f, 0x4);

        if (scale_offset) {
            g_dumper.add_offset("SpecialMesh", "Scale", *scale_offset);
        } else {
            spdlog::error("Failed to find Scale in SpecialMesh");
        }

        return true;
    }
} // namespace dumper::stages::special_mesh