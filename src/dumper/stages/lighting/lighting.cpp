#include "lighting.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/rtti/rtti.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::lighting {
    auto dump() -> bool {
        const auto sky = process::Rtti::find(dumper::g_lighting->get_address(), "Sky@RBX");

        if (!sky) {
            spdlog::error("Failed to find Sky in Lighting");
            return false;
        }

        dumper::g_dumper.add_offset("Lighting", "Sky", *sky);

        const auto atmosphere =
            process::Rtti::find(dumper::g_lighting->get_address(), "Atmosphere@RBX");

        if (!atmosphere) {
            spdlog::error("Failed to find Atmosphere in Lighting");
            return false;
        }

        dumper::g_dumper.add_offset("Lighting", "Atmosphere", *atmosphere);

        FIND_AND_ADD_OFFSET(dumper::g_lighting->get_address(), Lighting, float, Brightness, 3.567f,
                            0x400, 0x4);

        FIND_AND_ADD_OFFSET(dumper::g_lighting->get_address(), Lighting, uint64_t, ClockTime,
                            32400000000, 0x400, 0x8);

        FIND_AND_ADD_OFFSET(dumper::g_lighting->get_address(), Lighting, float,
                            EnvironmentDefuseScale, 0.678, 0x400, 0x4);

        FIND_AND_ADD_OFFSET(dumper::g_lighting->get_address(), Lighting, float,
                            EnvironmentSpecularScale, 0.762, 0x400, 0x4);

        FIND_AND_ADD_OFFSET(dumper::g_lighting->get_address(), Lighting, float, GeographicLatitude,
                            115.9, 0x400, 0x4);

        glm::vec3 ambient_color(72.0f / 255.0f, 69.0f / 255.0f, 11.0f / 255.0f);

        const auto ambient_offset = process::helpers::find_vec_offset<glm::vec3>(
            dumper::g_lighting->get_address(), ambient_color, 0x400, 0.01f, 0x4);

        if (ambient_offset) {
            g_dumper.add_offset("Lighting", "Ambient", *ambient_offset);
        } else {
            spdlog::error("Failed to find Ambient offset in Lighting");
        }

        glm::vec3 color_shift_bottom(10.0f / 255.0f, 12.0f / 255.0f, 29.0f / 255.0f);

        const auto color_shift_bottom_offset = process::helpers::find_vec_offset<glm::vec3>(
            dumper::g_lighting->get_address(), color_shift_bottom, 0x400, 0.01f, 0x4);

        if (color_shift_bottom_offset) {
            g_dumper.add_offset("Lighting", "ColorShift_Bottom", *color_shift_bottom_offset);
        } else {
            spdlog::error("Failed to find ColorShift_Bottom offset in Lighting");
        }

        glm::vec3 color_shift_top(19.0f / 255.0f, 4.0f / 255.0f, 45.0f / 255.0f);

        const auto color_shift_top_offset = process::helpers::find_vec_offset<glm::vec3>(
            dumper::g_lighting->get_address(), color_shift_top, 0x400, 0.01f, 0x4);

        if (color_shift_top_offset) {
            g_dumper.add_offset("Lighting", "ColorShift_Top", *color_shift_top_offset);
        } else {
            spdlog::error("Failed to find ColorShift_Top offset in Lighting");
        }

        glm::vec3 outdoor_ambient(71.0f / 255.0f, 44.0f / 255.0f, 30.0f / 255.0f);

        const auto outdoor_ambient_offset = process::helpers::find_vec_offset<glm::vec3>(
            dumper::g_lighting->get_address(), outdoor_ambient, 0x400, 0.01f, 0x4);

        if (outdoor_ambient_offset) {
            g_dumper.add_offset("Lighting", "OutdoorAmbient", *outdoor_ambient_offset);
        } else {
            spdlog::error("Failed to find OutdoorAmbient offset in Lighting");
        }

        return true;
    }

} // namespace dumper::stages::lighting