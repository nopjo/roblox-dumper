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

    void write_to_json(const std::string& filename = "offsets.json") {
        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(end_time - start_time).count();

        std::ofstream file(filename);
        if (!file.is_open()) {
            LOG_ERR("Failed to create JSON file: {}", filename);
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto time_t_val = std::chrono::system_clock::to_time_t(now);
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&time_t_val), "%Y-%m-%d %H:%M:%S");

        size_t total_offsets = 0;
        for (const auto& [ns, offsets] : namespaces)
            total_offsets += offsets.size();

        file << "{\n";
        file << "  \"metadata\": {\n";
        file << "    \"dumped_by\": \"Jonah's Roblox Dumper\",\n";
        file << "    \"github\": \"https://github.com/nopjo/roblox-dumper\",\n";
        file << "    \"contributors\": [\"@4hd8\", \"@fulore\"],\n";
        file << "    \"timestamp\": \"" << timestamp.str() << "\",\n";
        if (!roblox_version.empty()) {
            file << "    \"roblox_version\": \"" << roblox_version << "\",\n";
        }
        file << "    \"total_offsets\": " << total_offsets << ",\n";
        file << "    \"duration_seconds\": " << std::fixed << std::setprecision(2) << duration
             << "\n";
        file << "  },\n";
        file << "  \"offsets\": {\n";

        size_t ns_count = 0;
        for (const auto& namespace_name : namespace_order) {
            auto ns_it = namespaces.find(namespace_name);
            if (ns_it != namespaces.end()) {
                file << "    \"" << namespace_name << "\": {\n";

                size_t offset_count = 0;
                for (const auto& [offset_name, offset_value] : ns_it->second) {
                    file << "      \"" << offset_name << "\": " << std::dec << offset_value;
                    if (offset_count < ns_it->second.size() - 1) {
                        file << ",";
                    }
                    file << "\n";
                    offset_count++;
                }

                file << "    }";
                if (ns_count < namespace_order.size() - 1) {
                    file << ",";
                }
                file << "\n";
                ns_count++;
            }
        }

        file << "  }\n";
        file << "}\n";
        file.close();

        LOG_SUCCESS("Created: {} ({} offsets | {:.2f}s)", filename, total_offsets, duration);
    }

    void write_to_file(const std::string& filename = "offsets.hpp") {
        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(end_time - start_time).count();

        auto fflags_it = std::find(namespace_order.begin(), namespace_order.end(), "FFlags");
        if (fflags_it != namespace_order.end()) {
            namespace_order.erase(fflags_it);
            namespace_order.push_back("FFlags");
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            LOG_ERR("Failed to create file: {}", filename);
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto time_t_val = std::chrono::system_clock::to_time_t(now);
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&time_t_val), "%Y-%m-%d %H:%M:%S");

        size_t total_offsets = 0;
        for (const auto& [ns, offsets] : namespaces)
            total_offsets += offsets.size();

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
    }
};

#define offset_registry OffsetRegistry::instance()