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

    static auto find_attribute_offsets(uintptr_t instance, const std::string& first_attr_name,
                                       const std::string& first_attr_value,
                                       const std::string& second_attr_name,
                                       const std::string& second_attr_value)
        -> std::optional<std::tuple<size_t, size_t, size_t, size_t>> {
        size_t iterations = 0;
        constexpr size_t MAX_ITERATIONS = 50000;

        for (size_t container_off = 0; container_off < 0x200; container_off += 0x8) {
            auto container = process::Memory::read<uintptr_t>(instance + container_off);
            if (!container || *container < 0x10000) {
                continue;
            }

            for (size_t list_off = 0; list_off < 0x100; list_off += 0x8) {
                auto first_attr = process::Memory::read<uintptr_t>(*container + list_off);
                if (!first_attr || *first_attr < 0x10000) {
                    continue;
                }

                size_t value_off = 0;
                bool found_value = false;
                for (size_t off = 0; off < 0x100; off += 0x8) {
                    if (++iterations > MAX_ITERATIONS) {
                        spdlog::error("Attribute scan exceeded max iterations");
                        return std::nullopt;
                    }

                    auto str = process::Memory::read_sso_string(*first_attr + off);
                    if (str && *str == first_attr_value) {
                        value_off = off;
                        found_value = true;
                        break;
                    }
                }
                if (!found_value) {
                    continue;
                }

                uintptr_t second_attr = 0;
                size_t stride = 0;
                constexpr size_t MAX_STRIDE_SEARCH = 0x200;

                for (size_t offset = 0x8; offset < MAX_STRIDE_SEARCH; offset += 0x8) {
                    if (++iterations > MAX_ITERATIONS) {
                        spdlog::error("Attribute scan exceeded max iterations");
                        return std::nullopt;
                    }

                    uintptr_t test_addr = *first_attr + offset + value_off;
                    if (test_addr < 0x10000) {
                        break;
                    }

                    auto forward = process::Memory::read_sso_string(test_addr);
                    if (forward && *forward == second_attr_value) {
                        second_attr = *first_attr + offset;
                        stride = offset;
                        break;
                    }
                }

                if (!second_attr) {
                    continue;
                }

                for (size_t off = 0; off < 0x100; off += 0x8) {
                    if (++iterations > MAX_ITERATIONS) {
                        spdlog::error("Attribute scan exceeded max iterations");
                        return std::nullopt;
                    }

                    if (off == value_off) {
                        continue;
                    }

                    auto ptr = process::Memory::read<uintptr_t>(*first_attr + off);
                    if (!ptr || *ptr <= 0x10000) {
                        continue;
                    }

                    auto str = process::Memory::read_sso_string(*ptr);
                    if (str && *str == first_attr_name) {
                        return std::make_tuple(container_off, list_off, stride, value_off);
                    }
                }
            }
        }

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
                                                 "HelloWorld", "TestString2", "HelloWorld2");
        if (!attr) {
            spdlog::error("Failed to find Attribute offsets");
            return false;
        }

        dumper::g_dumper.add_offset("Instance", "AttributeContainer", std::get<0>(*attr));
        dumper::g_dumper.add_offset("Instance", "AttributeList", std::get<1>(*attr));
        dumper::g_dumper.add_offset("Instance", "AttributeToNext", std::get<2>(*attr));
        dumper::g_dumper.add_offset("Instance", "AttributeToValue", std::get<3>(*attr));

        return true;
    }
} // namespace dumper::stages::instance