#pragma once

#include <spdlog/spdlog.h>

namespace logger {
    auto initialize() -> void;
    auto print_error_summary() -> void;
} // namespace logger