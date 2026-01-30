#include "dumper.h"
#include "process/memory/memory.h"
#include "stages/atmosphere/atmosphere.h"
#include "stages/base_part/base_part.h"
#include "stages/bloom_effect/bloom_effect.h"
#include "stages/camera/camera.h"
#include "stages/character_mesh/character_mesh.h"
#include "stages/data_model/data_model.h"
#include "stages/gui_base2d/gui_base2d.h"
#include "stages/gui_object/gui_object.h"
#include "stages/humanoid/humanoid.h"
#include "stages/instance/instance.h"
#include "stages/lighting/lighting.h"
#include "stages/mesh_part/mesh_part.h"
#include "stages/mouse_service/mouse_service.h"
#include "stages/player/player.h"
#include "stages/proximity_prompt/proximity_prompt.h"
#include "stages/sky/sky.h"
#include "stages/special_mesh/special_mesh.h"
#include "stages/team/team.h"
#include "stages/terrain/terrain.h"
#include "stages/tool/tool.h"
#include "stages/value/value.h"
#include "stages/visual_engine/visual_engine.h"
#include "stages/workspace/workspace.h"
#include <cstring>
#include <fstream>
#include <spdlog/spdlog.h>
#include <thread>
#include <vector>

namespace dumper {

    Dumper::~Dumper() {}

    auto Dumper::start() -> bool {
        spdlog::info("Dumper starting.\n");

        if (!stages::visual_engine::dump()) {
            spdlog::error("Failed to dump VisualEngine");
            return false;
        }

        if (!stages::data_model::dump()) {
            spdlog::error("Failed to dump DataModel");
            return false;
        }

        if (!stages::instance::dump()) {
            spdlog::error("Failed to dump Instance");
            return false;
        }

        stages::workspace::dump();

        g_data_model = roblox::Instance(g_data_model_addr);
        g_lighting = g_data_model.find_first_child_of_class("Lighting");
        g_workspace = g_data_model.find_first_child_of_class("Workspace");

        stages::team::dump();

        std::vector<std::thread> threads;

        threads.emplace_back([]() { stages::player::dump(); });
        threads.emplace_back([]() { stages::base_part::dump(); });
        threads.emplace_back([]() { stages::humanoid::dump(); });
        threads.emplace_back([]() { stages::lighting::dump(); });
        threads.emplace_back([]() { stages::sky::dump(); });
        threads.emplace_back([]() { stages::mesh_part::dump(); });
        threads.emplace_back([]() { stages::atmosphere::dump(); });
        threads.emplace_back([]() { stages::bloom_effect::dump(); });
        threads.emplace_back([]() { stages::camera::dump(); });
        threads.emplace_back([]() { stages::special_mesh::dump(); });
        threads.emplace_back([]() { stages::terrain::dump(); });
        threads.emplace_back([]() { stages::gui_object::dump(); });
        threads.emplace_back([]() { stages::gui_base2d::dump(); });
        threads.emplace_back([]() { stages::value::dump(); });
        threads.emplace_back([]() { stages::mouse_service::dump(); });
        threads.emplace_back([]() { stages::character_mesh::dump(); });
        threads.emplace_back([]() { stages::tool::dump(); });
        threads.emplace_back([]() { stages::proximity_prompt::dump(); });

        for (auto& thread : threads) {
            thread.join();
        }

        return true;
    }

    auto Dumper::add_offset(const std::string& namespace_name, const std::string& offset_name,
                            size_t offset) -> void {
        std::lock_guard<std::mutex> lock(m_offset_mutex);
        m_offsets[namespace_name].push_back({offset_name, offset});
        spdlog::info("Added offset: {}::{} = 0x{:X}", namespace_name, offset_name, offset);
    }

    auto Dumper::get_offset(const std::string& namespace_name, const std::string& offset_name) const
        -> std::optional<size_t> {
        std::lock_guard<std::mutex> lock(m_offset_mutex);
        auto it = m_offsets.find(namespace_name);
        if (it == m_offsets.end()) {
            return std::nullopt;
        }

        for (const auto& entry : it->second) {
            if (entry.name == offset_name) {
                return entry.offset;
            }
        }

        return std::nullopt;
    }

} // namespace dumper