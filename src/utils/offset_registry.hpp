#pragma once
#include "sdk/offsets.hpp"
#include "utils/logger.hpp"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <map>
#include <regex>
#include <sstream>
#include <string>

class OffsetRegistry {
  private:
    std::map<std::string, std::map<std::string, uintptr_t>> namespaces;
    std::vector<std::string> namespace_order;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::string roblox_version;

    OffsetRegistry() { start_time = std::chrono::high_resolution_clock::now(); }

  public:
    static OffsetRegistry& instance() {
        static OffsetRegistry registry;
        return registry;
    }

    void add(const std::string& namespace_name, const std::string& offset_name, uintptr_t value) {
        auto it = std::find(namespace_order.begin(), namespace_order.end(), namespace_name);
        if (it == namespace_order.end()) {
            namespace_order.push_back(namespace_name);
        }
        namespaces[namespace_name][offset_name] = value;

        if (namespace_name == "Instance") {
            if (offset_name == "ClassDescriptor")
                sdk::offsets::Instance::ClassDescriptor = value;
            else if (offset_name == "Parent")
                sdk::offsets::Instance::Parent = value;
            else if (offset_name == "Name")
                sdk::offsets::Instance::Name = value;
            else if (offset_name == "ClassName")
                sdk::offsets::Instance::ClassName = value;
            else if (offset_name == "ChildrenStart")
                sdk::offsets::Instance::ChildrenStart = value;
            else if (offset_name == "ChildrenEnd")
                sdk::offsets::Instance::ChildrenEnd = value;
        }
        LOG_SUCCESS("{}: 0x{:X}", offset_name, value);
    }

    uintptr_t get(const std::string& namespace_name, const std::string& offset_name) const {
        auto it = namespaces.find(namespace_name);
        if (it == namespaces.end())
            return 0;

        auto off = it->second.find(offset_name);
        if (off == it->second.end())
            return 0;

        return off->second;
    }

    void set_roblox_version(const std::string& version) { roblox_version = version; }

    void write_to_file(const std::string& filename = "offsets.hpp") {
        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(end_time - start_time).count();

        auto fflags_it = std::find(namespace_order.begin(), namespace_order.end(), "FFlags");
        if (fflags_it != namespace_order.end()) {
            namespace_order.erase(fflags_it);
            namespace_order.push_back("FFlags");
        }

        auto now = std::chrono::system_clock::now();
        auto time_t_val = std::chrono::system_clock::to_time_t(now);
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&time_t_val), "%Y-%m-%d %H:%M:%S");

        size_t total_offsets = 0;
        for (const auto& [ns, offsets] : namespaces)
            total_offsets += offsets.size();

        std::ofstream file(filename);
        if (!file.is_open()) {
            LOG_ERR("Failed to create file: {}", filename);
            return;
        }

        file << "//================================================================\n";
        file << "// Dumped By Jonah's Roblox Dumper | Discord: jonahw\n";
        file << "// Github Link: https://github.com/nopjo/roblox-dumper\n";
        file << "// Contributors: @4hd8, @fulore \n";
        file << "// Dumped at: " << timestamp.str() << "\n";
        if (!roblox_version.empty()) {
            file << "// Roblox Version: " << roblox_version << "\n";
        }
        file << "// Total offsets dumped: " << total_offsets << "\n";
        file << "//================================================================\n\n";
        file << "#pragma once\n";
        file << "#include <cstdint>\n\n";
        file << "namespace offsets {\n";

        for (size_t i = 0; i < namespace_order.size(); i++) {
            const auto& namespace_name = namespace_order[i];
            auto ns_it = namespaces.find(namespace_name);
            if (ns_it != namespaces.end()) {
                file << "    namespace " << namespace_name << " {\n";
                for (const auto& [offset_name, offset_value] : ns_it->second) {
                    file << "        inline constexpr uintptr_t " << offset_name << " = 0x"
                         << std::hex << std::uppercase << offset_value << ";\n";
                }
                file << "    }";

                if (i < namespace_order.size() - 1) {
                    file << "\n\n";
                } else {
                    file << "\n";
                }
            }
        }

        file << "}\n";
        file.close();

        LOG_SUCCESS("Created: {} ({} offsets | {:.2f}s)", filename, total_offsets, duration);

        std::string json_filename = filename.substr(0, filename.find_last_of('.')) + ".json";
        std::ofstream json_file(json_filename);
        if (!json_file.is_open()) {
            LOG_ERR("Failed to create file: {}", json_filename);
            return;
        }

        json_file << "{\n";
        json_file << "    \"metadata\": {\n";
        json_file << "        \"dumper\": \"Jonah's Roblox Dumper\",\n";
        json_file << "        \"discord\": \"jonahw\",\n";
        json_file << "        \"github\": \"https://github.com/nopjo/roblox-dumper\",\n";
        json_file << "        \"contributors\": [\"@4hd8\", \"@fulore\"],\n";
        json_file << "        \"timestamp\": \"" << timestamp.str() << "\",\n";
        if (!roblox_version.empty()) {
            json_file << "        \"roblox_version\": \"" << roblox_version << "\",\n";
        }
        json_file << "        \"total_offsets\": " << std::dec << total_offsets << "\n";
        json_file << "    },\n";
        json_file << "    \"offsets\": {\n";

        for (size_t i = 0; i < namespace_order.size(); i++) {
            const auto& namespace_name = namespace_order[i];
            auto ns_it = namespaces.find(namespace_name);
            if (ns_it != namespaces.end()) {
                json_file << "        \"" << namespace_name << "\": {\n";

                size_t j = 0;
                for (const auto& [offset_name, offset_value] : ns_it->second) {
                    std::stringstream hex_value;
                    hex_value << "0x" << std::hex << std::uppercase << offset_value;

                    json_file << "            \"" << offset_name << "\": \"" << hex_value.str()
                              << "\"";

                    if (j < ns_it->second.size() - 1) {
                        json_file << ",";
                    }
                    json_file << "\n";
                    j++;
                }

                json_file << "        }";

                if (i < namespace_order.size() - 1) {
                    json_file << ",";
                }
                json_file << "\n";
            }
        }

        json_file << "    }\n";
        json_file << "}\n";
        json_file.close();

        LOG_SUCCESS("Created: {}", json_filename);
    }
};

#define offset_registry OffsetRegistry::instance()
