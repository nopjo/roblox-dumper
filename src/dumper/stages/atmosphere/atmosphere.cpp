#include "atmosphere.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"

namespace dumper::stages::atmosphere {
    auto dump() -> bool {
        const auto atmosphere = dumper::g_lighting->find_first_child_of_class("Atmosphere");

        if (!atmosphere) {
            spdlog::error("Failed to get Atmosphere instance from Lighting");
            return false;
        }

        FIND_AND_ADD_OFFSET(atmosphere->get_address(), Atmosphere, float, Density, 0.324f, 0x400,
                            0x4);

        FIND_AND_ADD_OFFSET(atmosphere->get_address(), Atmosphere, float, Offset, 0.561f, 0x400,
                            0x4);

        FIND_AND_ADD_OFFSET(atmosphere->get_address(), Atmosphere, float, Glare, 0.432f, 0x400,
                            0x4);

        FIND_AND_ADD_OFFSET(atmosphere->get_address(), Atmosphere, float, Haze, 0.123f, 0x400, 0x4);

        glm::vec3 color(100.0f / 255.0f, 186.0f / 255.0f, 199.0f / 255.0f);

        const auto color_offset = process::helpers::find_vec_offset<glm::vec3>(
            atmosphere->get_address(), color, 0x400, 0.01f, 0x4);

        if (color_offset) {
            g_dumper.add_offset("Atmosphere", "Color", *color_offset);
        } else {
            spdlog::error("Failed to find Color offset in Atmosphere");
        }

        glm::vec3 decay(106.0f / 255.0f, 112.0f / 255.0f, 125.0f / 255.0f);

        const auto decay_offset = process::helpers::find_vec_offset<glm::vec3>(
            atmosphere->get_address(), decay, 0x400, 0.01f, 0x4);

        if (decay_offset) {
            g_dumper.add_offset("Atmosphere", "Decay", *decay_offset);
        } else {
            spdlog::error("Failed to find Decay offset in Atmosphere");
        }

        return true;
    }

} // namespace dumper::stages::atmosphere