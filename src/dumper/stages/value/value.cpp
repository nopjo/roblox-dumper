#include "value.h"
#include "dumper/dumper.h"
#include "process/helpers/helpers.h"
#include <spdlog/spdlog.h>

namespace dumper::stages::value {
    auto dump() -> bool {
        const auto replicated_storage =
            dumper::g_data_model.find_first_child_of_class("ReplicatedStorage");

        if (!replicated_storage) {
            spdlog::error("Failed to find ReplicatedStorage in DataModel");
            return false;
        }

        const auto string_value = replicated_storage->find_first_child_of_class("StringValue");

        if (!string_value) {
            spdlog::error("Failed to find StringValue in ReplicatedStorage");
            return false;
        }

        const auto value_offset = process::helpers::find_sso_string_offset(
            string_value->get_address(), "we love jonah", 0x200, 0x8, true);

        if (!value_offset) {
            spdlog::error("Failed to dump Value for StringValue");
            return false;
        }

        dumper::g_dumper.add_offset("Value", "Value", *value_offset);

        return true;
    }
} // namespace dumper::stages::value