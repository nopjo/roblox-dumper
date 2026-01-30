#include "character_mesh.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::character_mesh {

    struct MeshData {
        std::string name;
        uintptr_t address;
        control::client::CharacterMeshProperty props;
    };

    static auto get_mesh_data(const control::client::CharacterMeshPropertiesInfo& props)
        -> std::optional<std::vector<MeshData>> {
        std::vector<MeshData> mesh_data;

        auto character_mesh_folder = dumper::g_workspace->find_first_child("CharacterMesh");
        if (!character_mesh_folder->is_valid()) {
            spdlog::error("Failed to find CharacterMesh folder");
            return std::nullopt;
        }

        for (const auto& prop : props.meshes) {
            const auto mesh = character_mesh_folder->find_first_child(prop.name);
            if (!mesh->is_valid()) {
                spdlog::error("Failed to find mesh: {}", prop.name);
                return std::nullopt;
            }

            MeshData data{.name = prop.name, .address = mesh->get_address(), .props = prop};

            mesh_data.push_back(data);
        }

        return mesh_data;
    }

    auto dump() -> bool {
        const auto mesh_props = control::client::g_client.get_character_mesh_properties();
        if (!mesh_props) {
            spdlog::error("Failed to get character mesh properties from control server");
            return false;
        }

        if (mesh_props->meshes.size() < 3) {
            spdlog::error("Not enough meshes found (need at least 3)");
            return false;
        }

        const auto meshes = get_mesh_data(*mesh_props);
        if (!meshes) {
            return false;
        }

        std::vector<uintptr_t> mesh_addrs;
        for (const auto& m : *meshes) {
            mesh_addrs.push_back(m.address);
        }

        const auto body_part_offset = process::helpers::find_offset_with_getter<uint8_t>(
            mesh_addrs, [&](size_t i) { return (*meshes)[i].props.body_part; }, 0x300, 0x1);
        if (!body_part_offset) {
            spdlog::error("Failed to find BodyPart offset");
            return false;
        }
        dumper::g_dumper.add_offset("CharacterMesh", "BodyPart", *body_part_offset);

        const auto base_texture_offset = process::helpers::find_offset_with_getter<uint64_t>(
            mesh_addrs, [&](size_t i) { return (*meshes)[i].props.base_texture_id; }, 0x300, 0x8);
        if (!base_texture_offset) {
            spdlog::error("Failed to find BaseTextureId offset");
            return false;
        }
        dumper::g_dumper.add_offset("CharacterMesh", "BaseTextureId", *base_texture_offset);

        const auto mesh_id_offset = process::helpers::find_offset_with_getter<uint64_t>(
            mesh_addrs, [&](size_t i) { return (*meshes)[i].props.mesh_id; }, 0x300, 0x8);
        if (!mesh_id_offset) {
            spdlog::error("Failed to find MeshId offset");
            return false;
        }
        dumper::g_dumper.add_offset("CharacterMesh", "MeshId", *mesh_id_offset);

        const auto overlay_texture_offset = process::helpers::find_offset_with_getter<uint64_t>(
            mesh_addrs, [&](size_t i) { return (*meshes)[i].props.overlay_texture_id; }, 0x300,
            0x8);
        if (!overlay_texture_offset) {
            spdlog::error("Failed to find OverlayTextureId offset");
            return false;
        }
        dumper::g_dumper.add_offset("CharacterMesh", "OverlayTextureId", *overlay_texture_offset);

        return true;
    }

} // namespace dumper::stages::character_mesh