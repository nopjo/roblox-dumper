#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "utils/offset_registry.hpp"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool BasePart(Context& ctx) {
        control::Controller controller("http://localhost:8000");
        constexpr int PROP_SLEEP_MS = 150;

        if (!ctx.workspace.is_valid()) {
            LOG_ERR("Workspace not valid");
            return false;
        }

        auto part1 = ctx.workspace.find_first_child("Part1");
        if (!part1.is_valid()) {
            LOG_ERR("Failed to find 'Part1' in Workspace");
            return false;
        }

        auto part2 = ctx.workspace.find_first_child("Part2");
        if (!part2.is_valid()) {
            LOG_ERR("Failed to find 'Part2' in Workspace");
            return false;
        }

        auto part3 = ctx.workspace.find_first_child("Part3");
        if (!part3.is_valid()) {
            LOG_ERR("Failed to find 'Part3' in Workspace");
            return false;
        }

        constexpr float PART1_POS_X = 0.0f;
        constexpr float PART1_POS_Y = 50.0f;
        constexpr float PART1_POS_Z = 0.0f;
        constexpr float PART1_SIZE_X = 4.0f;
        constexpr float PART1_SIZE_Y = 2.0f;
        constexpr float PART1_SIZE_Z = 8.0f;
        constexpr float PART1_TRANSPARENCY = 0.0f;
        constexpr float PART1_REFLECTANCE = 0.253f;
        constexpr uint8_t PART1_SHAPE = 0;
        constexpr uint16_t PART1_MATERIAL = 256;

        constexpr float PART2_POS_X = 20.0f;
        constexpr float PART2_POS_Y = 60.0f;
        constexpr float PART2_POS_Z = 30.0f;
        constexpr float PART2_SIZE_X = 10.0f;
        constexpr float PART2_SIZE_Y = 5.0f;
        constexpr float PART2_SIZE_Z = 3.0f;
        constexpr float PART2_TRANSPARENCY = 0.5f;
        constexpr float PART2_REFLECTANCE = 0.234f;
        constexpr uint8_t PART2_SHAPE = 1;
        constexpr uint16_t PART2_MATERIAL = 512;

        constexpr float PART3_POS_X = -15.0f;
        constexpr float PART3_POS_Y = 70.0f;
        constexpr float PART3_POS_Z = -25.0f;
        constexpr float PART3_SIZE_X = 6.0f;
        constexpr float PART3_SIZE_Y = 8.0f;
        constexpr float PART3_SIZE_Z = 12.0f;
        constexpr float PART3_TRANSPARENCY = 1.0f;
        constexpr float PART3_REFLECTANCE = 0.523f;
        constexpr uint8_t PART3_SHAPE = 2;
        constexpr uint16_t PART3_MATERIAL = 1088;

        // Roblox's weird color ordering: RGB visually, but stored as RBG, fuck roblox man
        constexpr uint8_t PART1_R = 255, PART1_B = 0, PART1_G = 0;
        constexpr uint8_t PART2_R = 0, PART2_B = 255, PART2_G = 0;
        constexpr uint8_t PART3_R = 0, PART3_B = 0, PART3_G = 255;

        constexpr size_t PRIMITIVE_SCAN_RANGE = 0x600;
        constexpr size_t PART_SCAN_RANGE = 0x300;
        constexpr size_t ALIGNMENT = 0x4;

        auto primitive_offset = memory->find_rtti_offset(part1.address, "Primitive@RBX");
        if (!primitive_offset) {
            LOG_ERR("Failed to find Primitive offset");
            return false;
        }
        offset_registry.add("Part", "Primitive", *primitive_offset);

        auto primitive1 = memory->read<uintptr_t>(part1.address + *primitive_offset);
        if (!primitive1) {
            LOG_ERR("Failed to read Primitive pointer for part1");
            return false;
        }

        auto primitive2 = memory->read<uintptr_t>(part2.address + *primitive_offset);
        if (!primitive2) {
            LOG_ERR("Failed to read Primitive pointer for part2");
            return false;
        }

        auto primitive3 = memory->read<uintptr_t>(part3.address + *primitive_offset);
        if (!primitive3) {
            LOG_ERR("Failed to read Primitive pointer for part3");
            return false;
        }

        auto pos_offset = memory->find_vector3_offset({primitive1, primitive2, primitive3},
                                                      {{PART1_POS_X, PART1_POS_Y, PART1_POS_Z},
                                                       {PART2_POS_X, PART2_POS_Y, PART2_POS_Z},
                                                       {PART3_POS_X, PART3_POS_Y, PART3_POS_Z}},
                                                      PRIMITIVE_SCAN_RANGE);

        if (!pos_offset) {
            LOG_ERR("Failed to find Position offset");
            return false;
        }
        offset_registry.add("Part", "Position", *pos_offset);

        offset_registry.add("Part", "Rotation", *pos_offset - 36);

        const auto size_offset =
            memory->find_vector3_offset({primitive1, primitive2, primitive3},
                                        {{PART1_SIZE_X, PART1_SIZE_Y, PART1_SIZE_Z},
                                         {PART2_SIZE_X, PART2_SIZE_Y, PART2_SIZE_Z},
                                         {PART3_SIZE_X, PART3_SIZE_Y, PART3_SIZE_Z}},
                                        PRIMITIVE_SCAN_RANGE);

        if (!size_offset) {
            LOG_ERR("Failed to find Size offset");
            return false;
        }
        offset_registry.add("Part", "Size", *size_offset);

        auto color_offset = memory->find_verified_offset<uint8_t>(
            {part1.address, part2.address, part3.address}, {PART1_R, PART2_R, PART3_R},
            PART_SCAN_RANGE, 0x1);

        if (!color_offset) {
            LOG_ERR("Failed to find Color3 offset");
            return false;
        }

        uint8_t b1 = memory->read<uint8_t>(part1.address + *color_offset + 1);
        uint8_t g1 = memory->read<uint8_t>(part1.address + *color_offset + 2);
        uint8_t b2 = memory->read<uint8_t>(part2.address + *color_offset + 1);
        uint8_t g2 = memory->read<uint8_t>(part2.address + *color_offset + 2);
        uint8_t b3 = memory->read<uint8_t>(part3.address + *color_offset + 1);
        uint8_t g3 = memory->read<uint8_t>(part3.address + *color_offset + 2);

        if (!(b1 == PART1_B && g1 == PART1_G && b2 == PART2_B && g2 == PART2_G && b3 == PART3_B &&
              g3 == PART3_G)) {
            LOG_ERR("Color3 verification failed");
            return false;
        }
        offset_registry.add("Part", "Color3", *color_offset);

        auto transparency_offset = memory->find_verified_offset_float(
            {part1.address, part2.address, part3.address},
            {PART1_TRANSPARENCY, PART2_TRANSPARENCY, PART3_TRANSPARENCY}, PART_SCAN_RANGE,
            ALIGNMENT);

        if (!transparency_offset) {
            LOG_ERR("Failed to find Transparency offset");
            return false;
        }
        offset_registry.add("Part", "Transparency", *transparency_offset);

        auto shape_offset = memory->find_verified_offset<uint8_t>(
            {part1.address, part2.address, part3.address}, {PART1_SHAPE, PART2_SHAPE, PART3_SHAPE},
            PART_SCAN_RANGE, 0x1);

        if (!shape_offset) {
            LOG_ERR("Failed to find Shape offset");
            return false;
        }
        offset_registry.add("Part", "Shape", *shape_offset);

        bool found_flags = false;
        for (size_t offset = 0; offset < 0x300; offset += 0x1) {
            uint8_t flags1 = memory->read<uint8_t>(primitive1 + offset);
            uint8_t flags2 = memory->read<uint8_t>(primitive2 + offset);
            uint8_t flags3 = memory->read<uint8_t>(primitive3 + offset);

            bool anchored_match =
                ((flags1 & 0x2) != 0) && ((flags2 & 0x2) != 0) && ((flags3 & 0x2) != 0);
            bool can_collide_match =
                ((flags1 & 0x8) == 0) && ((flags2 & 0x8) == 0) && ((flags3 & 0x8) != 0);
            bool can_touch_match =
                ((flags1 & 0x10) != 0) && ((flags2 & 0x10) == 0) && ((flags3 & 0x10) != 0);

            if (anchored_match && can_collide_match && can_touch_match) {
                offset_registry.add("Part", "PrimitiveFlags", offset);
                offset_registry.add("PrimitiveFlags", "Anchored", 0x2);
                offset_registry.add("PrimitiveFlags", "CanCollide", 0x8);
                offset_registry.add("PrimitiveFlags", "CanTouch", 0x10);
                found_flags = true;
                break;
            }
        }

        if (!found_flags) {
            LOG_ERR("Failed to find PrimitiveFlags offset");
            return false;
        }

        auto material_offset = memory->find_verified_offset<uint16_t>(
            {primitive1, primitive2, primitive3}, {PART1_MATERIAL, PART2_MATERIAL, PART3_MATERIAL},
            PRIMITIVE_SCAN_RANGE, 0x2);

        if (!material_offset) {
            LOG_ERR("Failed to find Material offset");
            return false;
        }
        offset_registry.add("Part", "Material", *material_offset);

        auto reflectance_offset = memory->find_verified_offset_float(
            {part1.address, part2.address, part3.address},
            {PART1_REFLECTANCE, PART2_REFLECTANCE, PART3_REFLECTANCE}, PART_SCAN_RANGE, ALIGNMENT);

        if (!reflectance_offset) {
            LOG_ERR("Failed to find Reflectance offset");
            return false;
        }
        offset_registry.add("Part", "Reflectance", *reflectance_offset);

        LOG_INFO("Scanning for CastShadow...");

        controller.set_part_cast_shadow("Part1", true);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> cast_shadow_values = {1, 0, 1, 0};
        auto cast_shadow_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            part1.address, cast_shadow_values,
            [&](size_t i) {
                controller.set_part_cast_shadow("Part1", cast_shadow_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            PART_SCAN_RANGE, 0x1);

        if (cast_shadow_offsets.empty()) {
            LOG_ERR("Failed to find CastShadow offset");
            return false;
        }
        offset_registry.add("Part", "CastShadow", cast_shadow_offsets[0]);

        LOG_INFO("Scanning for Locked...");

        controller.set_part_locked("Part1", false);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> locked_values = {0, 1, 0, 1};
        auto locked_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            part1.address, locked_values,
            [&](size_t i) {
                controller.set_part_locked("Part1", locked_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            PART_SCAN_RANGE, 0x1);

        if (locked_offsets.empty()) {
            LOG_ERR("Failed to find Locked offset");
            return false;
        }
        offset_registry.add("Part", "Locked", locked_offsets[0]);

        LOG_INFO("Scanning for Massless...");

        controller.set_part_massless("Part1", false);
        std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));

        std::vector<uint8_t> massless_values = {0, 1, 0, 1};
        auto massless_offsets = memory->find_offsets_with_snapshots<uint8_t>(
            part1.address, massless_values,
            [&](size_t i) {
                controller.set_part_massless("Part1", massless_values[i] == 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(PROP_SLEEP_MS));
            },
            PART_SCAN_RANGE, 0x1);

        if (massless_offsets.empty()) {
            LOG_ERR("Failed to find Massless offset");
            return false;
        }
        offset_registry.add("Part", "Massless", massless_offsets[0]);


        LOG_INFO("Part offset scan complete!");
        return true;
    }

} // namespace scanner::phases
