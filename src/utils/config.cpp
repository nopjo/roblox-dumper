#include "config.hpp"
#include "logger.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

namespace config {
    static Config g_config;
    static bool g_config_loaded = false;

    std::optional<Config> load_config(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return std::nullopt;
        }

        try {
            nlohmann::json j;
            file >> j;

            Config cfg;
            cfg.game_id = j.value("game_id", 0ULL);
            cfg.place_id = j.value("place_id", 0ULL);
            cfg.creator_id = j.value("creator_id", 0ULL);
            cfg.user_id = j.value("user_id", 0ULL);
            cfg.display_name = j.value("display_name", "");
            cfg.using_published_game = j.value("using_published_game", false);
            cfg.multithreaded_rtti_scan = j.value("multithreaded_rtti_scan", false);

            return cfg;
        } catch (const std::exception& e) {
            LOG_ERR("Failed to parse config file: {}", e.what());
            return std::nullopt;
        }
    }

    bool save_config(const Config& config, const std::string& filepath) {
        try {
            nlohmann::json j;
            j["game_id"] = config.game_id;
            j["place_id"] = config.place_id;
            j["creator_id"] = config.creator_id;
            j["user_id"] = config.user_id;
            j["display_name"] = config.display_name;
            j["using_published_game"] = config.using_published_game;
            j["multithreaded_rtti_scan"] = config.multithreaded_rtti_scan;

            std::ofstream file(filepath);
            if (!file.is_open()) {
                LOG_ERR("Failed to open config file for writing: {}", filepath);
                return false;
            }

            file << j.dump(4);
            return true;
        } catch (const std::exception& e) {
            LOG_ERR("Failed to save config file: {}", e.what());
            return false;
        }
    }

    static std::string read_line() {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    static bool read_yes_no(const std::string& prompt) {
        while (true) {
            std::cout << prompt << " (yes/no): ";
            std::string input = read_line();
            std::transform(input.begin(), input.end(), input.begin(), ::tolower);
            if (input == "yes" || input == "y") {
                return true;
            } else if (input == "no" || input == "n") {
                return false;
            } else {
                std::cout << "Please enter 'yes' or 'no'.\n";
            }
        }
    }

    static uint64_t read_uint64(const std::string& prompt) {
        while (true) {
            std::cout << prompt << ": ";
            std::string input = read_line();
            try {
                uint64_t value = std::stoull(input);
                return value;
            } catch (const std::exception&) {
                std::cout << "Please enter a valid number.\n";
            }
        }
    }

    static std::string read_string(const std::string& prompt) {
        std::cout << prompt << ": ";
        return read_line();
    }

    Config create_config() {
        std::cout << "----- Roblox Dumper Config | Made By Jonah -----\n";
        std::cout << "Github Link - https://github.com/nopjo/roblox-dumper\n\n";
        std::cout << "No configuration file found. Setup required!\n\n";

        Config cfg;

        bool using_published = read_yes_no("Are you using the published dumper Roblox game?");
        cfg.using_published_game = using_published;

        if (using_published) {
            cfg.game_id = 2647427431;
            cfg.place_id = 6923982205;
            cfg.creator_id = 2632766773;
            std::cout << "\nUsing published game IDs:\n";
            std::cout << "  Game ID: " << cfg.game_id << "\n";
            std::cout << "  Place ID: " << cfg.place_id << "\n";
            std::cout << "  Creator ID: " << cfg.creator_id << "\n\n";
        } else {
            std::cout << "\nPlease enter your custom game info:\n";
            cfg.game_id = read_uint64("Game ID");
            cfg.place_id = read_uint64("Place ID");
            cfg.creator_id = read_uint64("Creator ID");
        }

        std::cout << "\nPlease enter your user information:\n";
        cfg.user_id = read_uint64("User ID");
        cfg.display_name = read_string("Display Name");

        cfg.multithreaded_rtti_scan =
            read_yes_no("Use multi-threaded RTTI/pointer scanning? (no = safer, yes = faster)");

        std::cout << "\nConfiguration saved!\n";
        std::cout << "If you need to use a different configuration, delete 'dumper-config.json' "
                     "and restart.\n\n";

        return cfg;
    }

    bool init(const std::string& filepath) {
        auto loaded = load_config(filepath);
        if (loaded) {
            g_config = *loaded;
            g_config_loaded = true;
            LOG_INFO("Config loaded from {}", filepath);
            LOG_INFO("If you're using a different config, delete '{}' and restart.", filepath);
            return true;
        }

        g_config = create_config();
        g_config_loaded = true;

        if (!save_config(g_config, filepath)) {
            LOG_ERR("Failed to save config file");
            return false;
        }

        LOG_SUCCESS("Config saved to {}", filepath);
        return true;
    }

    const Config& get() {
        if (!g_config_loaded) {
            LOG_ERR("Config not initialised");
        }
        return g_config;
    }

    void set(const Config& cfg) {
        g_config = cfg;
        g_config_loaded = true;
    }
} // namespace config
