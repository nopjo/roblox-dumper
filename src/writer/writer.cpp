#include "writer.h"
#include "config.h"
#include "dumper/dumper.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <spdlog/spdlog.h>

namespace dumper::writer {

    auto IWriter::write(const std::string& filename, std::chrono::milliseconds elapsed_time)
        -> bool {
        try {
            std::filesystem::path exe_path = std::filesystem::current_path();

            std::string final_filename = filename;
            std::string ext = get_file_extension();
            if (final_filename.length() < ext.length() ||
                final_filename.compare(final_filename.length() - ext.length(), ext.length(), ext) !=
                    0) {
                final_filename += ext;
            }

            std::filesystem::path output_path = exe_path / final_filename;

            std::ofstream file(output_path);
            if (!file.is_open()) {
                spdlog::error("Failed to open file for writing: {}", output_path.string());
                return false;
            }

            file << generate_header_comment(elapsed_time);

            file << generate_content();

            file.close();

            spdlog::info("Successfully wrote offsets to: {}", output_path.string());
            return true;

        } catch (const std::exception& e) {
            spdlog::error("Failed to write file: {}", e.what());
            return false;
        }
    }

    auto IWriter::generate_header_comment(std::chrono::milliseconds elapsed_time) -> std::string {
        std::string comment = "/*\n";
        comment += " * Dumped With: " + std::string(PROJECT_NAME) + " " +
                   std::string(PROJECT_VERSION) + "\n";
        comment += " * Created by: Jonah (jonahw on Discord)\n";
        comment += " * Github: https://github.com/nopjo/roblox-dumper\n";
        comment += " * Time Taken: " + std::to_string(elapsed_time.count()) + " ms (" +
                   std::to_string(elapsed_time.count() / 1000.0) + " seconds)\n";
        comment += " * Total Offsets: " + std::to_string(get_total_offset_count()) + "\n";
        comment += " */\n\n";
        return comment;
    }

    auto IWriter::get_sorted_namespaces()
        -> std::vector<std::pair<std::string, std::vector<dumper::OffsetEntry>>> {
        const auto& offsets = g_dumper.m_offsets;

        std::vector<std::pair<std::string, std::vector<dumper::OffsetEntry>>> sorted_namespaces(
            offsets.begin(), offsets.end());

        std::sort(sorted_namespaces.begin(), sorted_namespaces.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });

        for (auto& [namespace_name, entries] : sorted_namespaces) {
            std::sort(entries.begin(), entries.end(),
                      [](const auto& a, const auto& b) { return a.name < b.name; });
        }

        return sorted_namespaces;
    }

    auto IWriter::get_total_offset_count() -> size_t {
        size_t total = 0;
        for (const auto& [namespace_name, entries] : g_dumper.m_offsets) {
            total += entries.size();
        }
        return total;
    }

    auto HeaderWriter::generate_content() -> std::string {
        std::string content = "#pragma once\n";
        content += "#include <cstdint>\n\n";
        content += "// clang-format off\n";
        content += "namespace offsets {\n";

        for (const auto& [namespace_name, entries] : get_sorted_namespaces()) {
            if (entries.empty()) {
                continue;
            }

            content += "    namespace " + namespace_name + " {\n";

            for (const auto& entry : entries) {
                content += "        inline constexpr uintptr_t " + entry.name + " = 0x" +
                           std::format("{:X}", entry.offset) + ";\n";
            }

            content += "    }\n\n";
        }

        content += "} // namespace offsets\n";
        return content;
    }

    auto JsonWriter::generate_header_comment(std::chrono::milliseconds elapsed_time)
        -> std::string {
        return "";
    }

    auto JsonWriter::generate_content() -> std::string {
        std::string content = "{\n";
        content += "  \"metadata\": {\n";
        content += "    \"dumper\": \"" + std::string(PROJECT_NAME) + " " +
                   std::string(PROJECT_VERSION) + "\",\n";
        content += "    \"author\": \"Jonah (jonahw on Discord)\",\n";
        content += "    \"github\": \"https://github.com/nopjo/roblox-dumper\",\n";
        content += "    \"total_offsets\": " + std::to_string(get_total_offset_count()) + "\n";
        content += "  },\n";
        content += "  \"offsets\": {\n";

        auto sorted_namespaces = get_sorted_namespaces();
        for (size_t i = 0; i < sorted_namespaces.size(); ++i) {
            const auto& [namespace_name, entries] = sorted_namespaces[i];
            if (entries.empty()) {
                continue;
            }

            content += "    \"" + namespace_name + "\": {\n";

            for (size_t j = 0; j < entries.size(); ++j) {
                const auto& entry = entries[j];
                content += "      \"" + entry.name + "\": " + std::to_string(entry.offset);
                if (j < entries.size() - 1) {
                    content += ",";
                }
                content += "\n";
            }

            content += "    }";
            if (i < sorted_namespaces.size() - 1) {
                content += ",";
            }
            content += "\n";
        }

        content += "  }\n";
        content += "}\n";
        return content;
    }

    auto PythonWriter::generate_content() -> std::string {
        std::string content = "";
        content += "class Offsets:\n";

        for (const auto& [namespace_name, entries] : get_sorted_namespaces()) {
            if (entries.empty()) {
                continue;
            }

            content += "    class " + namespace_name + ":\n";

            for (const auto& entry : entries) {
                content += "        " + entry.name + " = " + std::to_string(entry.offset) + "\n";
            }

            content += "\n";
        }

        return content;
    }

    auto PythonWriter::generate_header_comment(std::chrono::milliseconds elapsed_time)
        -> std::string {
        std::string comment = "# Dumped With: " + std::string(PROJECT_NAME) + " " +
                              std::string(PROJECT_VERSION) + "\n";
        comment += "# Created by: Jonah (jonahw on Discord)\n";
        comment += "# Github: https://github.com/nopjo/roblox-dumper\n";
        comment += "# Time Taken: " + std::to_string(elapsed_time.count()) + " ms (" +
                   std::to_string(elapsed_time.count() / 1000.0) + " seconds)\n";
        comment += "# Total Offsets: " + std::to_string(get_total_offset_count()) + "\n\n";
        return comment;
    }

    auto CSharpWriter::generate_content() -> std::string {
        std::string content = "using System;\n\n";
        content += "namespace RobloxOffsets\n{\n";

        for (const auto& [namespace_name, entries] : get_sorted_namespaces()) {
            if (entries.empty()) {
                continue;
            }

            content += "    public static class " + namespace_name + "\n    {\n";

            for (const auto& entry : entries) {
                content += "        public const ulong " + entry.name + " = 0x" +
                           std::format("{:X}", entry.offset) + ";\n";
            }

            content += "    }\n\n";
        }

        content += "} // namespace RobloxOffsets\n";
        return content;
    }

} // namespace dumper::writer