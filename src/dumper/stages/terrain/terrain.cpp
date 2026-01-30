#include "terrain.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::terrain {

    static auto find_material_color_offset(uintptr_t material_colors_data,
                                           size_t material_colors_size, uint8_t r, uint8_t g,
                                           uint8_t b) -> std::optional<size_t> {
        for (size_t material_idx = 0; material_idx < material_colors_size; material_idx++) {
            size_t color_offset = material_idx * 3;
            auto read_r = process::Memory::read<uint8_t>(material_colors_data + color_offset);
            auto read_g = process::Memory::read<uint8_t>(material_colors_data + color_offset + 1);
            auto read_b = process::Memory::read<uint8_t>(material_colors_data + color_offset + 2);

            if (read_r && read_g && read_b && *read_r == r && *read_g == g && *read_b == b) {
                return color_offset; // In my old version I did the index, but I think its easier
                                     // for people to just use the value offset
            }
        }
        return std::nullopt;
    }

    auto dump() -> bool {
        const auto terrain = dumper::g_workspace->find_first_child_of_class("Terrain");

        if (!terrain) {
            spdlog::error("Failed to find Terrain instance in Workspace");
            return false;
        }

        FIND_AND_ADD_OFFSET(terrain->get_address(), Terrain, float, GrassLength, 0.722f, 0x400,
                            0x4);

        FIND_AND_ADD_OFFSET(terrain->get_address(), Terrain, float, WaterReflectance, 0.935f, 0x400,
                            0x4);

        FIND_AND_ADD_OFFSET(terrain->get_address(), Terrain, float, WaterTransparency, 0.323f,
                            0x400, 0x4);

        FIND_AND_ADD_OFFSET(terrain->get_address(), Terrain, float, WaterWaveSize, 0.159f, 0x400,
                            0x4);

        FIND_AND_ADD_OFFSET(terrain->get_address(), Terrain, float, WaterWaveSpeed, 10.34f, 0x400,
                            0x4);

        const glm::vec3 water_color(12.0f / 255.0f, 84.0f / 255.0f, 92.0f / 255.0f);

        const auto water_color_offset = process::helpers::find_vec_offset<glm::vec3>(
            terrain->get_address(), water_color, 0x400, 0.01f, 0x4);

        if (!water_color_offset) {
            spdlog::error("Failed to find WaterColor in Terrain");
            return false;
        }

        dumper::g_dumper.add_offset("Terrain", "WaterColor", *water_color_offset);

        std::optional<size_t> material_colors_vec_offset;
        for (size_t vec_offset = 0x200; vec_offset < 0x500; vec_offset += 0x8) {
            const auto data_ptr =
                process::Memory::read<uintptr_t>(terrain->get_address() + vec_offset);
            if (!data_ptr || *data_ptr < 0x10000)
                continue;

            if (find_material_color_offset(*data_ptr, 100, 80, 84, 84)) {
                material_colors_vec_offset = vec_offset;
                break;
            }
        }

        if (!material_colors_vec_offset) {
            spdlog::error("Failed to find MaterialColors vector offset");
            return false;
        }

        dumper::g_dumper.add_offset("Terrain", "MaterialColors", *material_colors_vec_offset);

        const auto material_colors_data =
            process::Memory::read<uintptr_t>(terrain->get_address() + *material_colors_vec_offset);

        if (!material_colors_data || *material_colors_data < 0x10000) {
            spdlog::error("Failed to read MaterialColors data pointer");
            return false;
        }

        auto find_and_add = [&](const char* name, uint8_t r, uint8_t g, uint8_t b) {
            auto offset = find_material_color_offset(*material_colors_data, 100, r, g, b);
            if (offset) {
                dumper::g_dumper.add_offset("MaterialColors", name, *offset);
            } else {
                spdlog::warn("Failed to find {} material color offset", name);
            }
        };

        find_and_add("Asphalt", 80, 84, 84);
        find_and_add("Basalt", 75, 74, 74);
        find_and_add("Brick", 138, 97, 73);
        find_and_add("Cobblestone", 134, 134, 118);
        find_and_add("Concrete", 152, 152, 152);
        find_and_add("CrackedLava", 255, 24, 67);
        find_and_add("Glacier", 221, 228, 229);
        find_and_add("Grass", 111, 126, 62);
        find_and_add("Ground", 140, 130, 104);
        find_and_add("Ice", 204, 210, 223);
        find_and_add("LeafyGrass", 106, 134, 64);
        find_and_add("Limestone", 255, 243, 192);
        find_and_add("Mud", 121, 112, 98);
        find_and_add("Pavement", 143, 144, 135);
        find_and_add("Rock", 99, 100, 102);
        find_and_add("Salt", 255, 255, 254);
        find_and_add("Sand", 207, 203, 167);
        find_and_add("Sandstone", 148, 124, 95);
        find_and_add("Slate", 88, 89, 86);
        find_and_add("Snow", 235, 253, 255);
        find_and_add("WoodPlanks", 172, 148, 108);

        return true;
    }
} // namespace dumper::stages::terrain