#include "config.h"
#include "control/control.h"
#include "dumper/dumper.h"
#include "writer/writer.h"
#include <Windows.h>
#include <chrono>
#include <format>
#include <iostream>
#include <logger/logger.h>
#include <process/process.h>
#include <spdlog/spdlog.h>

auto main() -> int {
    logger::initialize();

    const auto title = std::format("{} {}", PROJECT_NAME, PROJECT_VERSION);

    spdlog::info("{} created by jonah/nopjo", title);
    spdlog::info("Github: https://github.com/nopjo/roblox-dumper\n");

    if (!process::g_process.attach("RobloxPlayerBeta.exe")) {
        MessageBoxA(
            nullptr,
            "Failed to attach to Roblox, please rerun the Dumper when Roblox has fully loaded.",
            title.c_str(), MB_OK | MB_ICONERROR);
        return 1;
    }

    spdlog::info("Attached to Roblox. PID: {}\n", process::g_process.get_pid());

    if (!control::g_control.start(8080)) {
        MessageBoxA(nullptr,
                    "Failed to start control server, make sure you have no applications running on "
                    "port 8080.",
                    title.c_str(), MB_OK | MB_ICONERROR);
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