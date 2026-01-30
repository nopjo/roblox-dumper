#include "team.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"

#include "spdlog/spdlog.h"

namespace dumper::stages::team {

    auto dump() -> bool {
        const auto teams = dumper::g_data_model.find_first_child_of_class("Teams");

        if (!teams) {
            spdlog::error("Failed to find Teams service");
            return false;
        }

        const auto team = teams->find_first_child_of_class("Team");

        if (!team) {
            spdlog::error("Failed to find Team instance inside Teams");
            return false;
        }

        // really red = 1004 (https://create.roblox.com/docs/reference/engine/datatypes/BrickColor)
        const auto team_color =
            process::helpers::find_offset<uint32_t>(team->get_address(), 1004, 0x600, 0x4);

        if (!team_color) {
            spdlog::error("Failed to find TeamColor in Team");
            return false;
        }

        dumper::g_dumper.add_offset("Team", "TeamColor", *team_color);

        dumper::g_team_addr = team->get_address();

        return true;
    }
} // namespace dumper::stages::team