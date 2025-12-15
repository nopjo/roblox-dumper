#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace config {
    struct Config {
        uint64_t game_id;
        uint64_t place_id;
        uint64_t creator_id;
        uint64_t user_id;
        std::string display_name;
        bool using_published_game;
        bool multithreaded_rtti_scan;
    };

    std::optional<Config> load_config(const std::string& filepath = "dumper-config.json");

    bool save_config(const Config& config, const std::string& filepath = "dumper-config.json");

    Config create_config();

    bool init(const std::string& filepath = "dumper-config.json");

    const Config& get();
    void set(const Config& cfg);
} // namespace config
