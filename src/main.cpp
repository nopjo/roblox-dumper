#include "config.h"
#include "control/control.h"
#include "dumper/dumper.h"
#include "writer/writer.h"
#include <chrono>
#include <format>
#include <iostream>
#include <logger/logger.h>
#include <process/process.h>
#include <spdlog/spdlog.h>

auto main() -> int {
    logger::initialize();

    const auto title = std::format("{} {}", PROJECT_NAME, PROJECT_VERSION);

    spdlog::info("{} created by jonah/nopjo (Linux port)", title);
    spdlog::info("Github: https://github.com/nopjo/roblox-dumper\n");

    if (!process::g_process.attach("sober")) {
        spdlog::error("Failed to attach to Roblox (Sober). Make sure:");
        spdlog::error("1. Roblox/Sober Flatpak is running");
        spdlog::error("2. This dumper is running with appropriate permissions");
        spdlog::error("3. Consider running with elevated privileges if needed");
        return 1;
    }

    spdlog::info("Attached to Roblox. PID: {}\n", process::g_process.get_pid());

    if (!control::g_control.start(8080)) {
        spdlog::error("Failed to start control server, make sure you have no applications running on port 8080.");
        return 1;
    }

    spdlog::info("Control server started on port 8080\n");

    const auto start_time = std::chrono::steady_clock::now();
    dumper::g_dumper.start();

    const auto end_time = std::chrono::steady_clock::now();
    const auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    spdlog::info("Finished in {} ms ({:.2f} seconds)", elapsed.count(), elapsed.count() / 1000.0);

    // will eventually have a config to choose what file types to dump
    dumper::writer::g_header_writer.write("offsets", elapsed);
    dumper::writer::g_json_writer.write("offsets", elapsed);
    dumper::writer::g_python_writer.write("offsets", elapsed);
    dumper::writer::g_csharp_writer.write("offsets", elapsed);

    logger::print_error_summary();

    return 0;
}
