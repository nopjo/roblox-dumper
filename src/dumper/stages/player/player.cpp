#include "player.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include "process/rtti/rtti.h"
#include "roblox/instance/instance.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::player {
    auto dump() -> bool {
        const auto players = dumper::g_data_model.find_first_child_of_class("Players");

        if (!players) {
            spdlog::error("Failed to find Players instance inside DataModel");
            return false;
        }

        const auto local_player = process::Rtti::find(players->get_address(), "Player@RBX");

        if (!local_player) {
            spdlog::error("Failed to find LocalPlayer in Players");
            return false;
        }

        dumper::g_dumper.add_offset("Players", "LocalPlayer", *local_player);

        const auto local_player_addr =
            process::Memory::read<uintptr_t>(players->get_address() + *local_player);

        const auto character = process::Rtti::find(*local_player_addr, "ModelInstance@RBX");
        if (!character) {
            spdlog::error("Failed to find Character offset for Player");
            return false;
        }

        dumper::g_dumper.add_offset("Player", "Character", *character);

        const auto player_info = control::client::g_client.get_player_information();

        if (!player_info) {
            spdlog::error("Failed to receive player information via control server.");
            return false;
        }

        FIND_AND_ADD_OFFSET(*local_player_addr, Player, uint64_t, UserId, player_info->user_id,
                            0x800, 0x8);

        const auto display_name = process::helpers::find_string_offset(
            *local_player_addr, player_info->display_name, 0x400, 0x8, 0x256, true);

        if (!display_name) {
            spdlog::error("Failed to get DisplayName from Player");
            return false;
        }

        dumper::g_dumper.add_offset("Player", "DisplayName", *display_name);

        FIND_AND_ADD_OFFSET(*local_player_addr, Player, uint32_t, AccountAge,
                            player_info->account_age, 0x600, 0x4);

        const auto locale_id = process::helpers::find_string_by_regex(
            *local_player_addr, R"([a-z]{2}-[a-z]{2})", 0x800, 0x8, 32, true);

        if (!locale_id) {
            spdlog::error("Failed to get LocaleId from Player");
            return false;
        }

        dumper::g_dumper.add_offset("Player", "LocaleId", *locale_id);

        // really red = 1004 (https://create.roblox.com/docs/reference/engine/datatypes/BrickColor)
        FIND_AND_ADD_OFFSET(*local_player_addr, Player, uint32_t, TeamColor, 1004, 0x800, 0x4);

        const auto team_ptr = process::helpers::find_pointer_offset(
            *local_player_addr, dumper::g_team_addr, 0x400, 0x8);

        if (!team_ptr) {
            spdlog::error("Failed to get Team from Player");
            return false;
        }

        dumper::g_dumper.add_offset("Player", "Team", *team_ptr);

        FIND_AND_ADD_OFFSET(*local_player_addr, Player, float, HealthDisplayDistance, 87.12f, 0x800,
                            0x4);
        FIND_AND_ADD_OFFSET(*local_player_addr, Player, float, NameDisplayDistance, 56.89f, 0x800,
                            0x4);

        return true;
    }
} // namespace dumper::stages::player