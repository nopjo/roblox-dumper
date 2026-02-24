#include "workspace.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include <process/helpers/helpers.h>
#include <process/rtti/rtti.h>
#include <spdlog/spdlog.h>

namespace dumper::stages::workspace {

    auto dump() -> bool {
        const auto camera = process::Rtti::find(dumper::g_workspace->get_address(), "Camera@RBX");
        if (!camera) {
            spdlog::error("Failed to find CurrentCamera offset in Workspace");
            return false;
        }

        dumper::g_dumper.add_offset("Workspace", "CurrentCamera", *camera);

        FIND_AND_ADD_OFFSET(dumper::g_workspace->get_address(), Workspace, float, ReadOnlyGravity,
                            196.2f, 0x1000, 0x4);

        const auto result = process::helpers::find_offset_in_pointer<float>(
            dumper::g_workspace->get_address(), 196.2f, 0x400, 0x200, 0x8, 0x4);

        if (!result) {
            spdlog::error("Failed to dump World and World Gravity in Workspace");
            return false;
        }

        const auto [world, gravity] = *result;
        g_dumper.add_offset("Workspace", "World", world);
        g_dumper.add_offset("World", "Gravity", gravity);

        const auto world_addr =
            process::Memory::read<uintptr_t>(g_workspace->get_address() + world);

        if (!world_addr) {
            spdlog::error("Failed to read World offset in Workspace");
            return false;
        }

        FIND_AND_ADD_OFFSET(*world_addr, World, float, WorldSteps, 240.0f, 0x1000, 0x4);

        std::optional<size_t> primitives_offset;

        for (size_t offset = 0; offset < 0x1000; offset += 0x8) {
            const auto ptr = process::Memory::read<uintptr_t>(*world_addr + offset);

            if (!ptr || *ptr == 0) {
                continue;
            }

            const auto rtti_offset = process::Rtti::find(*ptr, "Primitive@RBX");

            if (rtti_offset) {
                primitives_offset = offset;
                break;
            }
        }

        if (!primitives_offset) {
            spdlog::error("Failed to find Primitives offset in World");
            return false;
        }

        g_dumper.add_offset("World", "Primitives", *primitives_offset);

        return true;
    }
} // namespace dumper::stages::workspace