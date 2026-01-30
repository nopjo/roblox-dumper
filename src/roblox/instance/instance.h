#pragma once
#include "roblox/offsets.h"
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace roblox {
    class Instance {
      public:
        Instance() = default;
        explicit Instance(std::uint64_t address) : m_address(address){};

        auto is_valid() const -> bool;
        auto get_name() const -> std::optional<std::string>;
        auto get_class_name() const -> std::optional<std::string>;
        auto get_children() const -> std::vector<Instance>;
        auto get_parent() const -> std::optional<Instance>;
        auto find_first_child(std::string_view name) const -> std::optional<Instance>;

        template <typename T = Instance>
        auto find_first_child_of_class(std::string_view name) const -> std::optional<T> {
            for (const auto& child : get_children()) {
                const auto class_name = child.get_class_name();

                if (class_name && *class_name == name) {
                    return T(child.get_address());
                }
            }
            return std::nullopt;
        }

        auto get_address() const -> std::uint64_t;

      private:
        std::uint64_t m_address = 0;
    };
} // namespace roblox