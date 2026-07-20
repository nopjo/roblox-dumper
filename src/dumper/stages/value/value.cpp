#include "value.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include "roblox/offsets.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::value {
    auto dump() -> bool {
        const auto replicated_storage =
            dumper::g_data_model.find_first_child_of_class("ReplicatedStorage");

        if (!replicated_storage) {
            spdlog::error("Failed to find ReplicatedStorage in DataModel");
            return false;
        }

        const auto string_value = replicated_storage->find_first_child("TestValue");

        if (!string_value) {
            spdlog::error("Failed to find 'TestValue' StringValue in ReplicatedStorage");
            return false;
        }

        const auto value_offset = process::helpers::find_sso_string_offset(
            string_value->get_address(), "Test123", 0x100, 0x8, true);

        if (!value_offset) {
            spdlog::error("Failed to find Value offset for StringValue");
            return false;
        }

        dumper::g_dumper.add_offset("Value", "Value", *value_offset);
        roblox::offsets::Value::Value = *value_offset;

        return true;
    }
} // namespace dumper::stages::value
