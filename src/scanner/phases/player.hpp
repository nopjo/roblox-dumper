#pragma once
#include "control/control.hpp"
#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/offsets.hpp"
#include "settings.hpp"
#include "utils/offset_registry.hpp"
#include <chrono>
#include <thread>

namespace scanner::phases {

    inline bool player(Context& ctx) {
        const auto players = sdk::instance_t(ctx.data_model).find_first_child("Players");
        if (!players.is_valid()) {
            LOG_ERR("Players not found in DataModel");
            return false;
        }

        control::Controller controller("http://localhost:8000");

        auto local_player_offset = memory->find_rtti_offset(players.address, "Player@RBX");
        if (!local_player_offset) {
            LOG_ERR("Failed to find LocalPlayer offset");
            return false;
        }
        offset_registry.add("Players", "LocalPlayer", *local_player_offset);

        auto local_player = memory->read<uintptr_t>(players.address + *local_player_offset);
        if (!local_player) {
            LOG_ERR("Failed to read LocalPlayer pointer");
            return false;
        }

        const auto model = memory->find_rtti_offset(local_player, "ModelInstance@RBX");
        if (!model) {
            LOG_ERR("Failed to find ModelInstance offset");
            return false;
        }
        offset_registry.add("Player", "Character", *model);

        auto display_name =
            memory->find_roblox_string_direct(local_player, settings::display_name());
        if (!display_name) {
            LOG_ERR("Failed to find DisplayName offset");
            return false;
        }
        offset_registry.add("Player", "DisplayName", *display_name);

        auto user_id = memory->find_value_offset<uint64_t>(local_player, settings::user_id());
        if (!user_id) {
            LOG_ERR("Failed to find UserId offset");
            return false;
        }
        offset_registry.add("Player", "UserId", *user_id);

        controller.set_team("red");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        auto team_offset = memory->find_rtti_offset(local_player, "Team@RBX");

        // im so sorry for this code, i know there is better ways cba to change it tho
        for (size_t off = 0; off < 0x500; off += 0x8) {
            uintptr_t potential_team = memory->read<uintptr_t>(local_player + off);
            if (potential_team && potential_team > 0x10000) {
                uintptr_t vtable = memory->read<uintptr_t>(potential_team);
                if (vtable && vtable > 0x10000) {
                    uintptr_t class_desc = memory->read<uintptr_t>(
                        potential_team + sdk::offsets::Instance::ClassDescriptor);
                    if (class_desc && class_desc > 0x10000) {
                        uintptr_t name_ptr =
                            memory->read<uintptr_t>(class_desc + sdk::offsets::Instance::ClassName);
                        if (name_ptr) {
                            std::string class_name = memory->read_roblox_string(name_ptr);
                            if (class_name == "Team") {
                                team_offset = off;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (team_offset) {
            offset_registry.add("Player", "Team", *team_offset);
        } else {
            LOG_WARN("Failed to find Team offset");
        }

        return true;
    }

} // namespace scanner::phases