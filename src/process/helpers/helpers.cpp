#include "helpers.h"
#include "process/memory/memory.h"
#include "process/process.h"
#include "process/rtti/rtti.h"
#include <algorithm>
#include <spdlog/spdlog.h>

namespace process::helpers {
    auto find_pointer_by_rtti(std::string_view section_name,
                              const std::vector<std::string>& class_names, size_t alignment)
        -> std::unordered_map<std::string, std::optional<size_t>> {
        std::unordered_map<std::string, std::optional<size_t>> results;

        for (const auto& class_name : class_names) {
            results[class_name] = std::nullopt;
        }

        auto section = process::g_process.get_section(section_name);
        if (!section) {
            spdlog::error("Failed to find section: {}", section_name);
            return results;
        }

        auto [section_start, section_size] = *section;
        auto module_base = process::g_process.get_module_base();

        for (size_t offset = 0; offset < section_size; offset += alignment) {
            auto potential_ptr = process::Memory::read<uintptr_t>(section_start + offset);
            if (!potential_ptr || *potential_ptr < 0x10000) {
                continue;
            }

            auto rtti = process::Rtti::scan_rtti(*potential_ptr);
            if (!rtti) {
                continue;
            }

            for (const auto& class_name : class_names) {
                if (rtti->name == class_name && !results[class_name]) {
                    size_t final_offset = (section_start + offset) - module_base;
                    results[class_name] = final_offset;
                }
            }

            bool all_found = std::all_of(results.begin(), results.end(),
                                         [](const auto& pair) { return pair.second.has_value(); });
            if (all_found) {
                break;
            }
        }

        for (const auto& [class_name, offset] : results) {
            if (!offset) {
                spdlog::warn("Failed to find class: {}", class_name);
            }
        }

        return results;
    }

    auto find_sso_string_offset(uintptr_t base_address, const std::string& target_string,
                                size_t max_offset, size_t alignment, bool direct)
        -> std::optional<size_t> {
        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            if (direct) {
                auto str = Memory::read_sso_string(base_address + offset);
                if (str && *str == target_string) {
                    return offset;
                }
            } else {
                auto string_ptr = Memory::read<uintptr_t>(base_address + offset);
                if (!string_ptr || *string_ptr < 0x10000) {
                    continue;
                }

                auto str = Memory::read_sso_string(*string_ptr);
                if (str && *str == target_string) {
                    return offset;
                }
            }
        }

        return std::nullopt;
    }

    auto find_string_offset(uintptr_t base_address, const std::string& target_string,
                            size_t max_offset, size_t alignment, size_t max_string_length,
                            bool direct) -> std::optional<size_t> {
        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            if (direct) {
                auto str = Memory::read_string(base_address + offset, max_string_length);
                if (str && *str == target_string) {
                    return offset;
                }
            } else {
                auto string_ptr = Memory::read<uintptr_t>(base_address + offset);
                if (!string_ptr || *string_ptr < 0x10000) {
                    continue;
                }

                auto str = Memory::read_string(*string_ptr, max_string_length);
                if (str && *str == target_string) {
                    return offset;
                }
            }
        }

        return std::nullopt;
    }

    auto find_string_by_regex(uintptr_t base_address, const std::string& regex_pattern,
                              size_t max_offset, size_t alignment, size_t max_string_length,
                              bool direct) -> std::optional<size_t> {
        std::regex pattern(regex_pattern);

        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            if (direct) {
                auto str = Memory::read_string(base_address + offset, max_string_length);
                if (str && std::regex_match(*str, pattern)) {
                    return offset;
                }
            } else {
                auto string_ptr = Memory::read<uintptr_t>(base_address + offset);
                if (!string_ptr || *string_ptr < 0x10000) {
                    continue;
                }

                auto str = Memory::read_string(*string_ptr, max_string_length);
                if (str && std::regex_match(*str, pattern)) {
                    return offset;
                }
            }
        }

        return std::nullopt;
    }

    auto find_pointer_offset(uintptr_t base_address, uintptr_t target_pointer, size_t max_offset,
                             size_t alignment) -> std::optional<size_t> {
        for (size_t offset = 0; offset < max_offset; offset += alignment) {
            auto ptr = Memory::read<uintptr_t>(base_address + offset);
            if (ptr && *ptr == target_pointer) {
                return offset;
            }
        }
        return std::nullopt;
    }

    auto find_color3_offset(const std::vector<uintptr_t>& addresses,
                            std::function<std::tuple<uint8_t, uint8_t, uint8_t>(size_t)> get_rgb,
                            size_t max_offset) -> std::optional<size_t> {
        for (size_t offset = 0; offset < max_offset; offset += 1) {
            bool all_match = true;

            for (size_t i = 0; i < addresses.size(); i++) {
                auto [exp_r, exp_g, exp_b] = get_rgb(i);

                auto r = Memory::read<uint8_t>(addresses[i] + offset);
                auto g = Memory::read<uint8_t>(addresses[i] + offset + 1);
                auto b = Memory::read<uint8_t>(addresses[i] + offset + 2);

                if (!r || !g || !b || *r != exp_r || *g != exp_g || *b != exp_b) {
                    all_match = false;
                    break;
                }
            }

            if (all_match) {
                return offset;
            }
        }

        return std::nullopt;
    }

} // namespace process::helpers