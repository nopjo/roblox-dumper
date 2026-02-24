#include "vehicle_seat.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::vehicle_seat {
    auto dump() -> bool {
        const auto vehicle_seat = g_workspace->find_first_child_of_class("VehicleSeat");

        if (!vehicle_seat) {
            spdlog::error("Failed to find VehicleSeat in Workspace");
            return false;
        }

        FIND_AND_ADD_OFFSET(vehicle_seat->get_address(), VehicleSeat, float, MaxSpeed, 456.2f,
                            0x1000, 0x4);
        FIND_AND_ADD_OFFSET(vehicle_seat->get_address(), VehicleSeat, float, SteerFloat, 0.256f,
                            0x1000, 0x4);
        FIND_AND_ADD_OFFSET(vehicle_seat->get_address(), VehicleSeat, float, ThrottleFloat, 0.412f,
                            0x1000, 0x4);
        FIND_AND_ADD_OFFSET(vehicle_seat->get_address(), VehicleSeat, float, Torque, 108.1f, 0x1000,
                            0x4);
        FIND_AND_ADD_OFFSET(vehicle_seat->get_address(), VehicleSeat, float, TurnSpeed, 26.123f,
                            0x1000, 0x4);

        const auto vehicle_npc_hum = dumper::g_workspace->find_first_child("VehicleNPC")
                                         ->find_first_child_of_class("Humanoid");

        if (!vehicle_npc_hum) {
            spdlog::error("Failed to find VehicleNPC.Humanoid in Workspace");
        }

        const auto occupant = process::helpers::find_pointer_offset(
            vehicle_seat->get_address(), vehicle_npc_hum->get_address(), 0x1000, 0x8);

        if (!occupant) {
            spdlog::error("Failed to find VehicleSeat Occupant offset");
            return false;
        }

        g_dumper.add_offset("VehicleSeat", "Occupant", *occupant);

        return true;
    }

} // namespace dumper::stages::vehicle_seat