#include "visual_engine.h"
#include "dumper/dumper.h"
#include "dumper/macros.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include "process/rtti/rtti.h"
#include <array>
#include <spdlog/spdlog.h>

namespace dumper::stages::visual_engine {

    static auto get_pointers() -> bool {
        const auto results = process::helpers::find_pointer_by_rtti(
            ".data", {"VisualEngine@Graphics@RBX", "DataModel@RBX"});

        if (!results.at("VisualEngine@Graphics@RBX")) {
            spdlog::error("Failed to find VisualEngine pointer");
            return false;
        }

        if (!results.at("DataModel@RBX")) {
            spdlog::error("Failed to find DataModel pointer");
            return false;
        }

        g_dumper.add_offset("VisualEngine", "Pointer", *results.at("VisualEngine@Graphics@RBX"));
        g_dumper.add_offset("FakeDataModel", "Pointer", *results.at("DataModel@RBX"));

        return true;
    }

    static auto dump_render_view(const uintptr_t visual_engine) -> bool {
        const auto render_view_offset =
            process::Rtti::find(visual_engine, "RenderView@Graphics@RBX");

        if (!render_view_offset) {
            spdlog::error("Failed to get RenderView offset from VisualEngine");
            return false;
        }

        g_dumper.add_offset("VisualEngine", "RenderView", *render_view_offset);

        const auto render_view =
            process::Memory::read<uintptr_t>(visual_engine + *render_view_offset);

        FIND_AND_ADD_OFFSET(*render_view, RenderView, uint16_t, LightingValid, 257, 0x300, 0x2);

        return true;
    };

    static auto dump_view_matrix(const uintptr_t visual_engine) -> bool {
        auto is_valid_view_matrix = [](const float mat[16]) -> bool {
            if (std::abs(mat[11] - 0.1f) > 0.01f)
                return false;

            if (std::abs(mat[14] + 1.0f) < 0.01f && std::abs(mat[15]) < 0.01f) {
                return false; // thiss is projection matrix
            }

            if (std::abs(mat[15]) < 10.0f || std::abs(mat[15]) > 10000.0f) {
                return false;
            }

            for (int i = 0; i < 16; i++) {
                if (std::isnan(mat[i]) || std::isinf(mat[i])) {
                    return false;
                }
            }

            return true;
        };

        for (size_t offset = 0; offset < 0x2000; offset += 0x10) {
            float mat[16];
            bool valid_read = true;

            for (int i = 0; i < 16; i++) {
                auto val = process::Memory::read<float>(visual_engine + offset + (i * 4));
                if (!val) {
                    valid_read = false;
                    break;
                }
                mat[i] = *val;
            }

            if (!valid_read)
                continue;

            if (is_valid_view_matrix(mat)) {
                g_dumper.add_offset("VisualEngine", "ViewMatrix", offset);
                return true;
            }
        }

        spdlog::error("Failed to find ViewMatrix");
        return false;
    }

    auto dump() -> bool {
        if (!get_pointers()) {
            return false;
        }

        const auto visual_engine = process::Memory::read<uintptr_t>(
            process::g_process.get_module_base() + *g_dumper.get_offset("VisualEngine", "Pointer"));

        dump_render_view(*visual_engine);
        dump_view_matrix(*visual_engine);

        const auto window_dimensions = process::g_process.get_window_dimensions();

        if (!window_dimensions) {
            spdlog::error("Failed to get window dimensions");
            return false;
        }

        const auto dimensions_offset =
            process::helpers::find_vec_offset(*visual_engine, *window_dimensions, 0x1000, 5.0f);

        if (!dimensions_offset) {
            spdlog::error("Failed to get Dimensions offset from VisualEngine");
            return false;
        }

        g_dumper.add_offset("VisualEngine", "Dimensions", *dimensions_offset);

        const auto fake_data_model = process::Rtti::find(*visual_engine, "DataModel@RBX");

        if (!fake_data_model) {
            spdlog::error("Failed to get FakeDataModel offset from VisualEngine");
            return false;
        }

        g_dumper.add_offset("VisualEngine", "FakeDataModel", *fake_data_model);

        return true;
    }
} // namespace dumper::stages::visual_engine
