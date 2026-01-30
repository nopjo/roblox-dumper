#include "base_part.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include "process/rtti/rtti.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace dumper::stages::base_part {

    struct PartData {
        std::string name;
        uintptr_t address;
        uintptr_t primitive_address;
        control::client::PartProperty props;
    };

    static auto get_part_data(const control::client::PartPropertiesInfo& props,
                              size_t primitive_offset) -> std::optional<std::vector<PartData>> {
        std::vector<PartData> part_data;

        for (const auto& prop : props.parts) {
            const auto part = dumper::g_workspace->find_first_child(prop.name);
            if (!part->is_valid()) {
                spdlog::error("Failed to find part: {}", prop.name);
                return std::nullopt;
            }

            auto primitive_ptr =
                process::Memory::read<uintptr_t>(part->get_address() + primitive_offset);

            if (!primitive_ptr) {
                spdlog::error("Failed to read Primitive pointer for {}", prop.name);
                return std::nullopt;
            }

            PartData data{.name = prop.name,
                          .address = part->get_address(),
                          .primitive_address = *primitive_ptr,
                          .props = prop};

            part_data.push_back(data);
        }

        return part_data;
    }

    static auto find_primitive_offset() -> std::optional<size_t> {
        auto initial_props = control::client::g_client.get_part_properties();
        if (!initial_props || initial_props->parts.empty()) {
            spdlog::error("Failed to get part properties from control server");
            return std::nullopt;
        }

        auto first_part = dumper::g_workspace->find_first_child(initial_props->parts[0].name);
        if (!first_part->is_valid()) {
            spdlog::error("Failed to find part: {}", initial_props->parts[0].name);
            return std::nullopt;
        }

        const auto primitive_offset =
            process::Rtti::find(first_part->get_address(), "Primitive@RBX");

        if (!primitive_offset) {
            spdlog::error("Failed to find Primitive offset in BasePart");
            return std::nullopt;
        }

        return primitive_offset;
    }

    static auto find_primitive_flags_offset(const std::vector<PartData>& parts)
        -> std::optional<size_t> {
        for (size_t offset = 0; offset < 0x300; offset += 0x1) {
            bool all_match = true;
            bool found_variation = false;

            std::vector<uint8_t> flag_values;

            for (const auto& part : parts) {
                auto flags = process::Memory::read<uint8_t>(part.primitive_address + offset);

                if (!flags) {
                    all_match = false;
                    break;
                }

                flag_values.push_back(*flags);

                bool is_anchored = (*flags & 0x2) != 0;
                bool is_can_collide = (*flags & 0x8) != 0;
                bool is_can_touch = (*flags & 0x10) != 0;

                if (is_anchored != part.props.anchored ||
                    is_can_collide != part.props.can_collide ||
                    is_can_touch != part.props.can_touch) {
                    all_match = false;
                    break;
                }
            }

            if (!all_match) {
                continue;
            }

            for (size_t i = 1; i < flag_values.size(); i++) {
                if (flag_values[i] != flag_values[0]) {
                    found_variation = true;
                    break;
                }
            }

            if (all_match && found_variation) {
                return offset;
            }
        }

        return std::nullopt;
    }

    auto dump() -> bool {
        const auto primitive_offset = find_primitive_offset();
        if (!primitive_offset) {
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Primitive", *primitive_offset);

        const auto part_props = control::client::g_client.get_part_properties();
        if (!part_props) {
            spdlog::error("Failed to get part properties from control server");
            return false;
        }

        if (part_props->parts.size() < 3) {
            spdlog::error("Not enough parts found (need at least 3)");
            return false;
        }

        const auto parts = get_part_data(*part_props, *primitive_offset);
        if (!parts) {
            return false;
        }

        std::vector<uintptr_t> part_addrs, primitive_addrs;
        for (const auto& p : *parts) {
            part_addrs.push_back(p.address);
            primitive_addrs.push_back(p.primitive_address);
        }

        const auto flags_offset = find_primitive_flags_offset(*parts);
        if (!flags_offset) {
            spdlog::error("Failed to find PrimitiveFlags offset");
            return false;
        }
        dumper::g_dumper.add_offset("Primitive", "PrimitiveFlags", *flags_offset);
        dumper::g_dumper.add_offset("PrimitiveFlags", "Anchored", 0x2);
        dumper::g_dumper.add_offset("PrimitiveFlags", "CanCollide", 0x8);
        dumper::g_dumper.add_offset("PrimitiveFlags", "CanTouch", 0x10);

        const auto position_offset =
            process::helpers::find_vec3_offset_multi<glm::vec3>(primitive_addrs, [&](size_t i) {
                const auto& p = (*parts)[i].props;
                return glm::vec3(p.pos_x, p.pos_y, p.pos_z);
            });
        if (!position_offset) {
            spdlog::error("Failed to find Position offset");
            return false;
        }
        dumper::g_dumper.add_offset("Primitive", "Position", *position_offset);

        const auto cframe_offset = *position_offset - 36;
        dumper::g_dumper.add_offset("Primitive", "CFrame", cframe_offset);
        dumper::g_dumper.add_offset("Primitive", "Rotation", cframe_offset);
        dumper::g_dumper.add_offset("Primitive", "Orientation", cframe_offset);

        const auto size_offset =
            process::helpers::find_vec3_offset_multi<glm::vec3>(primitive_addrs, [&](size_t i) {
                const auto& p = (*parts)[i].props;
                return glm::vec3(p.size_x, p.size_y, p.size_z);
            });
        if (!size_offset) {
            spdlog::error("Failed to find Size offset");
            return false;
        }
        dumper::g_dumper.add_offset("Primitive", "Size", *size_offset);

        const auto material_offset = process::helpers::find_offset_with_getter<uint16_t>(
            primitive_addrs, [&](size_t i) { return (*parts)[i].props.material; }, 0x600, 0x2);
        if (!material_offset) {
            spdlog::error("Failed to find Material offset");
            return false;
        }
        dumper::g_dumper.add_offset("Primitive", "Material", *material_offset);

        const auto color_offset = process::helpers::find_color3_offset(part_addrs, [&](size_t i) {
            const auto& p = (*parts)[i].props;
            return std::make_tuple(p.color_r, p.color_g, p.color_b);
        });
        if (!color_offset) {
            spdlog::error("Failed to find Color3 offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Color3", *color_offset);

        const auto transparency_offset = process::helpers::find_offset_with_getter<float>(
            part_addrs, [&](size_t i) { return (*parts)[i].props.transparency; }, 0x300, 0x4);
        if (!transparency_offset) {
            spdlog::error("Failed to find Transparency offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Transparency", *transparency_offset);

        const auto reflectance_offset = process::helpers::find_offset_with_getter<float>(
            part_addrs, [&](size_t i) { return (*parts)[i].props.reflectance; }, 0x300, 0x4);
        if (!reflectance_offset) {
            spdlog::error("Failed to find Reflectance offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Reflectance", *reflectance_offset);

        const auto cast_shadow_offset = process::helpers::find_offset_with_getter<uint8_t>(
            part_addrs, [&](size_t i) { return (*parts)[i].props.cast_shadow ? 1 : 0; }, 0x300,
            0x1);
        if (!cast_shadow_offset) {
            spdlog::error("Failed to find CastShadow offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "CastShadow", *cast_shadow_offset);

        const auto locked_offset = process::helpers::find_offset_with_getter<uint8_t>(
            part_addrs, [&](size_t i) { return (*parts)[i].props.locked ? 1 : 0; }, 0x300, 0x1);
        if (!locked_offset) {
            spdlog::error("Failed to find Locked offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Locked", *locked_offset);

        const auto shape_offset = process::helpers::find_offset_with_getter<uint8_t>(
            part_addrs, [&](size_t i) { return (*parts)[i].props.shape; }, 0x300, 0x1);
        if (!shape_offset) {
            spdlog::error("Failed to find Shape offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Shape", *shape_offset);

        const auto massless_offset = process::helpers::find_offset_with_getter<uint8_t>(
            part_addrs, [&](size_t i) { return (*parts)[i].props.massless ? 1 : 0; }, 0x300, 0x1);
        if (!massless_offset) {
            spdlog::error("Failed to find Massless offset");
            return false;
        }
        dumper::g_dumper.add_offset("BasePart", "Massless", *massless_offset);

        const auto walking_npc = dumper::g_workspace->find_first_child("WalkingNpc");
        if (!walking_npc->is_valid()) {
            spdlog::error("Failed to find WalkingNpc");
            return false;
        }

        const auto npc_head = walking_npc->find_first_child("Head");
        if (!npc_head->is_valid()) {
            spdlog::error("Failed to find Head in WalkingNpc");
            return false;
        }

        auto npc_primitive_ptr =
            process::Memory::read<uintptr_t>(npc_head->get_address() + *primitive_offset);
        if (!npc_primitive_ptr) {
            spdlog::error("Failed to read Primitive pointer for NPC Head");
            return false;
        }

        glm::vec3 expected_linear_velocity(-0.062f, 0.0f, 0.496f);
        const auto linear_velocity_offset = process::helpers::find_vec_offset(
            *npc_primitive_ptr, expected_linear_velocity, 0x200, 0.1f, 0x4);

        if (!linear_velocity_offset) {
            spdlog::error("Failed to find AssemblyLinearVelocity offset");
            return false;
        }

        dumper::g_dumper.add_offset("Primitive", "AssemblyLinearVelocity", *linear_velocity_offset);
        dumper::g_dumper.add_offset("Primitive", "AssemblyAngularVelocity",
                                    *linear_velocity_offset + 0xC);

        return true;
    }

} // namespace dumper::stages::base_part