#pragma once
#include <cstdint>
#include <mutex>
#include <optional>
#include <roblox/instance/instance.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace dumper {
    struct OffsetEntry {
        std::string name;
        size_t offset;
    };

    class Dumper {
      public:
        Dumper() = default;
        ~Dumper();

        auto start() -> bool;
        auto add_offset(const std::string& namespace_name, const std::string& offset_name,
                        size_t offset) -> void;
        auto get_offset(const std::string& namespace_name, const std::string& offset_name) const
            -> std::optional<size_t>;

        std::unordered_map<std::string, std::vector<OffsetEntry>> m_offsets;

      private:
        mutable std::mutex m_offset_mutex;
    };

    inline Dumper g_dumper;

    inline uintptr_t g_visual_engine;
    inline uintptr_t g_data_model_addr;
    inline uintptr_t g_team_addr;

    inline roblox::Instance g_data_model;
    inline std::optional<roblox::Instance> g_lighting;
    inline std::optional<roblox::Instance> g_workspace;
} // namespace dumper