#include "sky.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/rtti/rtti.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::sky {
    static auto dump_texture_ids(const roblox::Instance& sky) -> void {
        struct TextureIdInfo {
            std::string name;
            std::string asset_id;
        };

        const std::vector<TextureIdInfo> texture_ids = {
            {"MoonTextureId", "rbxassetid://6444320592"},
            {"SkyboxBk", "rbxassetid://6444884337"},
            {"SkyboxDn", "rbxassetid://6444884785"},
            {"SkyboxFt", "rbxassetid://6444884336"},
            {"SkyboxLf", "rbxassetid://6444824337"},
            {"SkyboxRt", "rbxassetid://6444884331"},
            {"SkyboxUp", "rbxassetid://6412503613"},
            {"SunTextureId", "rbxassetid://6196665106"}};

        for (const auto& [name, asset_id] : texture_ids) {
            const auto offset = process::helpers::find_sso_string_offset(
                sky.get_address(), asset_id, 0x800, 0x8, true);

            if (offset) {
                g_dumper.add_offset("Sky", name, *offset);
            } else {
                spdlog::error("Failed to find {} offset in Sky", name);
            }
        }
    }

    auto dump() -> bool {
        const auto sky = dumper::g_lighting->find_first_child_of_class("Sky");

        if (!sky) {
            spdlog::error("Failed to find Sky instance in Lighting");
            return false;
        }

        FIND_AND_ADD_OFFSET(sky->get_address(), Sky, float, MoonAngularSize, 22.56f, 0x800, 0x4);

        FIND_AND_ADD_OFFSET(sky->get_address(), Sky, int, StarCount, 2346, 0x800, 0x4);

        FIND_AND_ADD_OFFSET(sky->get_address(), Sky, float, SunAngularSize, 11.98f, 0x800, 0x4);

        glm::vec3 skybox_orientation(90.2f, 12.2f, 4.0f);

        const auto skybox_orientation_offset = process::helpers::find_vec_offset<glm::vec3>(
            sky->get_address(), skybox_orientation, 0x400, 1.0f, 0x4);

        if (skybox_orientation_offset) {
            g_dumper.add_offset("Sky", "SkyboxOrientation", *skybox_orientation_offset);
        } else {
            spdlog::error("Failed to find Sky offset in Atmosphere");
        }

        dump_texture_ids(*sky);

        return true;
    }

} // namespace dumper::stages::sky