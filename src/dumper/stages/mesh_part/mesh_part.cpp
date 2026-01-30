#include "mesh_part.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"

#include <spdlog/spdlog.h>

namespace dumper::stages::mesh_part {

    auto dump() -> bool {
        const auto mesh_part = dumper::g_workspace->find_first_child_of_class("MeshPart");

        if (!mesh_part) {
            spdlog::error("Failed to find 'Mesh' in workspace.");
        }

        const auto mesh_id = process::helpers::find_sso_string_offset(
            mesh_part->get_address(), "rbxassetid://5547037341", 0x800, 0x8, true);

        if (!mesh_id) {
            spdlog::error("Failed to get MeshId for MeshPart");
            return false;
        }

        dumper::g_dumper.add_offset("MeshPart", "MeshId", *mesh_id);

        const auto texture_id = process::helpers::find_sso_string_offset(
            mesh_part->get_address(), "rbxassetid://5547037342", 0x800, 0x8, true);

        if (!texture_id) {
            spdlog::error("Failed to get TextureId for MeshPart");
            return false;
        }

        dumper::g_dumper.add_offset("MeshPart", "TextureId", *texture_id);

        return true;
    }
} // namespace dumper::stages::mesh_part