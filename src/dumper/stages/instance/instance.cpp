#include "instance.h"
#include "control/client/client.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include "process/memory/memory.h"
#include "process/rtti/rtti.h"
#include "roblox/offsets.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::instance {

    static auto find_children_offsets(uintptr_t instance, size_t expected_count,
                                      size_t parent_offset)
        -> std::optional<std::pair<size_t, size_t>> {
        for (size_t start_off = 0; start_off < 0x200; start_off += 0x8) {
            if (start_off == parent_offset) {
                continue;
            }

            auto start_ptr = process::Memory::read<uintptr_t>(instance + start_off);
            if (!start_ptr || *start_ptr < 0x10000) {
                continue;
            }

            for (size_t end_off = 0; end_off < 0x20; end_off += 0x8) {
                auto end_ptr = process::Memory::read<uintptr_t>(*start_ptr + end_off);
                if (!end_ptr || *end_ptr < 0x10000) {
                    continue;
                }

                size_t count = 0;
                auto node_opt = process::Memory::read<uintptr_t>(*start_ptr);
                if (!node_opt) {
                    continue;
                }

                uintptr_t node = *node_opt;
                for (int i = 0; i < 1000 && node != *end_ptr; i++, node += 0x10) {
                    auto child = process::Memory::read<uintptr_t>(node);
                    if (!child || *child < 0x10000) {
                        break;
                    }

                    auto vtable = process::Memory::read<uintptr_t>(*child);
                    if (!vtable || *vtable < 0x10000) {
                        break;
                    }

                    count++;
                }

                if (count == expected_count) {
                    return std::make_pair(start_off, end_off);
                }
            }
        }

        return std::nullopt;
    }

    static auto find_attribute_offsets(uintptr_t instance, const std::string& key1,
                                       const std::string& val1, const std::string& key2)
        -> std::optional<std::tuple<size_t, size_t, size_t, size_t>> {
        size_t component_map_off = 0;
        uintptr_t comp_map = 0;

        for (size_t off = 0; off < 0x200; off += 0x8) {
            auto comp = process::Memory::read<uintptr_t>(instance + off);
            if (!comp || *comp < 0x10000)
                continue;

            auto start = process::Memory::read<uintptr_t>(*comp);
            auto end = process::Memory::read<uintptr_t>(*comp + 0x8);
            if (!start || !end || *end <= *start || (*end - *start) > 0x1000)
                continue;

            for (uintptr_t idx = 0; idx < *end - *start; idx += 0x10) {
                auto entry = process::Memory::read<uintptr_t>(*start + idx);
                if (!entry || *entry < 0x10000)
                    continue;

                auto listing = process::Memory::read<uintptr_t>(*entry + 0x10);
                if (!listing || *listing < 0x10000)
                    continue;

                auto key_ptr = process::Memory::read<uintptr_t>(*listing + 0x0);
                if (!key_ptr || *key_ptr < 0x10000)
                    continue;

                auto key_str = process::Memory::read_string(*key_ptr);
                if (!key_str || key_str->empty() || key_str->length() > 128)
                    continue;

                component_map_off = off;
                comp_map = *comp;
                goto found;
            }
        }

        spdlog::error("Failed to find ComponentMap offset");
        return std::nullopt;

    found:
        auto start = process::Memory::read<uintptr_t>(comp_map);
        auto end = process::Memory::read<uintptr_t>(comp_map + 0x8);
        if (!start || !end) {
            spdlog::error("Failed to re-read component map bounds");
            return std::nullopt;
        }

        for (uintptr_t idx = 0; idx < *end - *start; idx += 0x10) {
            auto entry = process::Memory::read<uintptr_t>(*start + idx);
            if (!entry || *entry < 0x10000)
                continue;

            auto listing = process::Memory::read<uintptr_t>(*entry + 0x10);
            if (!listing || *listing < 0x10000)
                continue;

            size_t key_off = SIZE_MAX;
            for (size_t off = 0; off < 0x80; off += 0x8) {
                auto ptr = process::Memory::read<uintptr_t>(*listing + off);
                if (!ptr || *ptr < 0x10000)
                    continue;
                auto str = process::Memory::read_string(*ptr);
                if (str && *str == key1) {
                    key_off = off;
                    break;
                }
            }
            if (key_off == SIZE_MAX)
                continue;

            size_t val_off = SIZE_MAX;
            for (size_t off = 0; off < 0x80; off += 0x8) {
                if (off == key_off)
                    continue;
                auto str = process::Memory::read_sso_string(*listing + off);
                if (str && *str == val1) {
                    val_off = off;
                    break;
                }
            }
            if (val_off == SIZE_MAX)
                continue;

            uintptr_t addr1 = SIZE_MAX, addr2 = SIZE_MAX;
            for (size_t step = 0; step < 0x400; step += 0x8) {
                auto ptr = process::Memory::read<uintptr_t>(*listing + step + key_off);
                if (!ptr || *ptr < 0x10000)
                    continue;
                auto str = process::Memory::read_string(*ptr);
                if (!str)
                    continue;
                if (*str == key1 && addr1 == SIZE_MAX)
                    addr1 = *listing + step;
                if (*str == key2 && addr2 == SIZE_MAX)
                    addr2 = *listing + step;
                if (addr1 != SIZE_MAX && addr2 != SIZE_MAX)
                    break;
            }
            if (addr1 == SIZE_MAX || addr2 == SIZE_MAX)
                continue;

            size_t stride = addr2 - addr1;

            return std::make_tuple(component_map_off, key_off, val_off, stride);
        }

        spdlog::error("Failed to find attribute offsets for '{}'", key1);
        return std::nullopt;
    }

    auto dump() -> bool {
        const auto workspace_addr = process::Memory::read<uintptr_t>(
            dumper::g_data_model_addr + *dumper::g_dumper.get_offset("DataModel", "Workspace"));

        if (!workspace_addr) {
            spdlog::error("Failed to read Workspace from Datamodel");
            return false;
        }

        const auto instance_name =
            process::helpers::find_sso_string_offset(*workspace_addr, "Workspace");

        if (!instance_name) {
            spdlog::error("Failed to get Name for Instance");
            return false;
        }

        dumper::g_dumper.add_offset("Instance", "Name", *instance_name);

        const auto class_descriptor =
            process::Rtti::find(*workspace_addr, "ClassDescriptor@Reflection@RBX");

        if (!class_descriptor) {
            spdlog::error("Failed to get ClassDescriptor for Instance");
            return false;
        }

        dumper::g_dumper.add_offset("Instance", "ClassDescriptor", *class_descriptor);

        const auto class_descriptor_addr =
            process::Memory::read<uintptr_t>(*workspace_addr + *class_descriptor);

        if (!class_descriptor_addr) {
            spdlog::error("Failed to read ClassDescriptor for Instance");
            return false;
        }

        const auto class_name =
            process::helpers::find_sso_string_offset(*class_descriptor_addr, "Workspace");

        if (!class_name) {
            spdlog::error("Failed to get ClassName for Instance");
            return false;
        }

        dumper::g_dumper.add_offset("Instance", "ClassName", *class_name);

        const auto parent = process::Rtti::find(*workspace_addr, "DataModel@RBX");

        if (!parent) {
            spdlog::error("Failed to get Parent for Instance");
            return false;
        }

        dumper::g_dumper.add_offset("Instance", "Parent", *parent);

        const auto workspace_info = control::client::g_client.get_workspace_information();

        if (!workspace_info) {
            spdlog::error("Failed to receive Workspace information via control server.");
            return false;
        }

        const auto children =
            find_children_offsets(*workspace_addr, workspace_info->children_count, *parent);

        if (!children) {
            spdlog::error("Failed to find Children offsets");
            return false;
        }

        dumper::g_dumper.add_offset("Instance", "ChildrenStart", children->first);
        dumper::g_dumper.add_offset("Instance", "ChildrenEnd", children->second);

        roblox::offsets::Instance::Name = *instance_name;
        roblox::offsets::Instance::ClassDescriptor = *class_descriptor;
        roblox::offsets::Instance::ClassName = *class_name;
        roblox::offsets::Instance::Parent = *parent;
        roblox::offsets::Instance::ChildrenStart = children->first;
        roblox::offsets::Instance::ChildrenEnd = children->second;

        dumper::g_workspace = roblox::Instance(*workspace_addr);

        const auto attributes = dumper::g_workspace->find_first_child("Attributes");
        if (!attributes->is_valid()) {
            spdlog::error("Failed to find 'Attributes' Part in Workspace");
            return false;
        }

        const auto attr = find_attribute_offsets(attributes->get_address(), "TestString",
                                                 "HelloWorld", "TestString2");
        if (attr) {
            dumper::g_dumper.add_offset("Instance", "ComponentMap", std::get<0>(*attr));
            dumper::g_dumper.add_offset("Attribute", "Key", std::get<1>(*attr));
            dumper::g_dumper.add_offset("Attribute", "Value", std::get<2>(*attr));
            dumper::g_dumper.add_offset("Attribute", "Size", std::get<3>(*attr));
            return true;
        } else {
            spdlog::error("Failed to find Attribute offsets");
            // not fatal if fails
        }

        return true;
    }
} // namespace dumper::stages::instance