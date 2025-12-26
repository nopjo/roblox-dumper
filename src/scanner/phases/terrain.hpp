#pragma once
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/offset_registry.hpp"
#include <cmath>
#include <optional>
#include <string>
#include <vector>

namespace scanner::phases {

    inline std::optional<size_t> find_material_colors_vec_offset(uintptr_t terrain_address,
                                                                 uint8_t test_r, uint8_t test_g,
                                                                 uint8_t test_b) {
        for (size_t vec_offset = 0x200; vec_offset < 0x500; vec_offset += 0x8) {
            uintptr_t data_ptr = memory->read<uintptr_t>(terrain_address + vec_offset);
            if (!data_ptr || data_ptr < 0x10000)
                continue;

            for (size_t material_idx = 0; material_idx < 100; material_idx++) {
                size_t color_offset = material_idx * 3;
                uint8_t r = memory->read<uint8_t>(data_ptr + color_offset);
                uint8_t g = memory->read<uint8_t>(data_ptr + color_offset + 1);
                uint8_t b = memory->read<uint8_t>(data_ptr + color_offset + 2);

                if (r == test_r && g == test_g && b == test_b) {
                    return vec_offset;
                }
            }
        }
        return std::nullopt;
    }

    inline bool terrain(Context& ctx) {
        if (!ctx.workspace.is_valid()) {
            LOG_ERR("Workspace not valid");
            return false;
        }

        const auto terrain_inst = ctx.workspace.find_first_child_of_class("Terrain");
        if (!terrain_inst.is_valid()) {
            LOG_ERR("Failed to find Terrain in Workspace");
            return false;
        }

        const auto grass_length_offset =
            memory->find_verified_offset_float({terrain_inst.address}, {0.723f}, 0x1000, 0x4);

        if (!grass_length_offset) {
            LOG_ERR("Failed to find GrassLength offset for Terrain");
            return false;
        }
        offset_registry.add("Terrain", "GrassLength", *grass_length_offset);

        /*
        scan for the Asphalt colour first in the "std::vector<RBX::Color3uint8> materialColors"
        there was also baseMaterialColors but i believe this is used for storing the default
        material colors
        */
        auto material_colors_vec_offset =
            find_material_colors_vec_offset(terrain_inst.address, 80, 84, 84);

        if (!material_colors_vec_offset) {
            LOG_ERR("Failed to find MaterialColors vector pointer offset for Terrain");
            return false;
        }

        offset_registry.add("Terrain", "MaterialColors", *material_colors_vec_offset);

        uintptr_t material_colors_data =
            memory->read<uintptr_t>(terrain_inst.address + *material_colors_vec_offset);
        if (!material_colors_data || material_colors_data < 0x10000) {
            LOG_ERR("Failed to read MaterialColors data pointer");
            return false;
        }

        size_t material_colors_size =
            memory->read<size_t>(terrain_inst.address + *material_colors_vec_offset + 0x8);
        if (material_colors_size == 0 || material_colors_size > 200) {
            material_colors_size = 100;
        }

        auto find_material_index = [&](uint8_t r, uint8_t g, uint8_t b) -> std::optional<size_t> {
            for (size_t material_idx = 0; material_idx < material_colors_size; material_idx++) {
                size_t color_offset = material_idx * 3;
                uint8_t read_r = memory->read<uint8_t>(material_colors_data + color_offset);
                uint8_t read_g = memory->read<uint8_t>(material_colors_data + color_offset + 1);
                uint8_t read_b = memory->read<uint8_t>(material_colors_data + color_offset + 2);

                if (read_r == r && read_g == g && read_b == b) {
                    return material_idx;
                }
            }
            return std::nullopt;
        };

        auto find_and_add_color = [&](const char* name, uint8_t r, uint8_t g, uint8_t b) -> bool {
            auto material_idx = find_material_index(r, g, b);
            if (material_idx) {
                offset_registry.add("MaterialColors", name, *material_idx);
                return true;
            } else {
                LOG_ERR("Failed to find {} material index for Terrain", name);
                return false;
            }
        };

        find_and_add_color("Asphalt", 80, 84, 84);
        find_and_add_color("Basalt", 75, 74, 74);
        find_and_add_color("Brick", 138, 97, 73);
        find_and_add_color("Cobblestone", 134, 134, 118);
        find_and_add_color("Concrete", 152, 152, 152);
        find_and_add_color("CrackedLava", 255, 24, 67);
        find_and_add_color("Glacier", 221, 228, 229);
        find_and_add_color("Grass", 111, 126, 62);
        find_and_add_color("Ground", 140, 130, 104);
        find_and_add_color("Ice", 204, 210, 223);
        find_and_add_color("LeafyGrass", 106, 134, 64);
        find_and_add_color("Limestone", 255, 243, 192);
        find_and_add_color("Mud", 121, 112, 98);
        find_and_add_color("Pavement", 143, 144, 135);
        find_and_add_color("Rock", 99, 100, 102);
        find_and_add_color("Salt", 255, 255, 254);
        find_and_add_color("Sand", 207, 203, 167);
        find_and_add_color("Sandstone", 148, 124, 95);
        find_and_add_color("Slate", 88, 89, 86);
        find_and_add_color("Snow", 235, 253, 255);
        find_and_add_color("WoodPlanks", 172, 148, 108);

        constexpr float WATER_COLOR_R = 12.0f / 255.0f;
        constexpr float WATER_COLOR_G = 84.0f / 255.0f;
        constexpr float WATER_COLOR_B = 92.0f / 255.0f;

        auto water_color_offset = memory->find_verified_offset_float({terrain_inst.address},
                                                                     {WATER_COLOR_R}, 0x1000, 0x4);

        if (water_color_offset) {
            float g = memory->read<float>(terrain_inst.address + *water_color_offset + 4);
            float b = memory->read<float>(terrain_inst.address + *water_color_offset + 8);
            if (std::abs(g - WATER_COLOR_G) < 0.01f && std::abs(b - WATER_COLOR_B) < 0.01f) {
                offset_registry.add("Terrain", "WaterColor", *water_color_offset);
            } else {
                LOG_ERR("Failed to verify WaterColor G/B channels for Terrain");
            }
        } else {
            LOG_ERR("Failed to find WaterColor offset for Terrain");
        }

        const auto water_reflectance_offset =
            memory->find_verified_offset_float({terrain_inst.address}, {0.345f}, 0x1000, 0x4);

        if (!water_reflectance_offset) {
            LOG_ERR("Failed to find WaterReflectance offset for Terrain");
            return false;
        }
        offset_registry.add("Terrain", "WaterReflectance", *water_reflectance_offset);

        const auto water_transparency_offset =
            memory->find_verified_offset_float({terrain_inst.address}, {0.812f}, 0x1000, 0x4);

        if (!water_transparency_offset) {
            LOG_ERR("Failed to find WaterTransparency offset for Terrain");
            return false;
        }
        offset_registry.add("Terrain", "WaterTransparency", *water_transparency_offset);

        const auto water_wave_size_offset =
            memory->find_verified_offset_float({terrain_inst.address}, {0.156f}, 0x1000, 0x4);

        if (!water_wave_size_offset) {
            LOG_ERR("Failed to find WaterWaveSize offset for Terrain");
            return false;
        }
        offset_registry.add("Terrain", "WaterWaveSize", *water_wave_size_offset);

        const auto water_wave_speed_offset =
            memory->find_verified_offset_float({terrain_inst.address}, {30.34f}, 0x1000, 0x4);

        if (!water_wave_speed_offset) {
            LOG_ERR("Failed to find WaterWaveSpeed offset for Terrain");
            return false;
        }
        offset_registry.add("Terrain", "WaterWaveSpeed", *water_wave_speed_offset);

        return true;
    }

} // namespace scanner::phases
