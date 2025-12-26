#include "memory/memory.h"
#include "scanner.hpp"
#include "sdk/instance.hpp"
#include "utils/logger.hpp"
#include "utils/offset_registry.hpp"
#include "utils/structs.h"
#include <Windows.h>
#include <algorithm>
#include <chrono>
#include <thread>

namespace scanner::phases {
    inline void move_mouse_in_window(HWND hwnd, int x, int y) {
        if (!hwnd || !IsWindow(hwnd))
            return;

        if (IsIconic(hwnd)) {
            ShowWindow(hwnd, SW_RESTORE);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        POINT client_point = {x, y};
        ClientToScreen(hwnd, &client_point);
        int screen_x = client_point.x;
        int screen_y = client_point.y;

        int screen_width = GetSystemMetrics(SM_CXSCREEN);
        int screen_height = GetSystemMetrics(SM_CYSCREEN);

        int abs_x = (screen_x * 65535) / screen_width;
        int abs_y = (screen_y * 65535) / screen_height;

        SetCursorPos(screen_x, screen_y);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        INPUT input = {};
        input.type = INPUT_MOUSE;
        input.mi.dx = abs_x;
        input.mi.dy = abs_y;
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        SendInput(1, &input, sizeof(INPUT));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        LPARAM lParam = MAKELPARAM(x, y);
        for (int i = 0; i < 3; i++) {
            PostMessage(hwnd, WM_MOUSEMOVE, 0, lParam);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        SendMessage(hwnd, WM_MOUSEMOVE, 0, lParam);
    }
    inline bool mouse_service(Context& ctx) {
        if (!ctx.data_model) {
            LOG_ERR("DataModel not valid");
            return false;
        }

        const auto mouse_service =
            sdk::instance_t(ctx.data_model).find_first_child_of_class("MouseService");

        if (!mouse_service.is_valid()) {
            LOG_ERR("Could not find 'MouseService' inside data model.");
            return false;
        }

        // use second occurrence not first.
        auto input_object_offset =
            memory->find_rtti_offset_nth(mouse_service.address, "InputObject@RBX", 1);
        if (!input_object_offset) {
            LOG_ERR("Failed to find InputObject offset (second occurrence)");
            return false;
        }

        offset_registry.add("MouseService", "InputObject", *input_object_offset);

        uintptr_t input_object_ptr =
            memory->read<uintptr_t>(mouse_service.address + *input_object_offset);
        if (!input_object_ptr || input_object_ptr < 0x10000) {
            LOG_ERR("Failed to read InputObject pointer");
            return false;
        }

        HWND roblox_window = memory->get_window_handle();
        if (!roblox_window) {
            LOG_ERR("Failed to get Roblox window handle");
            return false;
        }

        RECT client_rect;
        if (!GetClientRect(roblox_window, &client_rect)) {
            LOG_ERR("Failed to get client rect");
            return false;
        }

        int client_width = client_rect.right - client_rect.left;
        int client_height = client_rect.bottom - client_rect.top;

        std::vector<Vector2> mouse_positions = {
            {100.0f, 100.0f},
            {500.0f, 300.0f},
            {200.0f, 400.0f},
        };

        constexpr int SLEEP_MS = 1100;

        move_mouse_in_window(roblox_window, static_cast<int>(mouse_positions[0].x),
                             static_cast<int>(mouse_positions[0].y));
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

        auto offsets = memory->find_offsets_with_snapshots<Vector2>(
            input_object_ptr, mouse_positions,
            [&](size_t i) {
                move_mouse_in_window(roblox_window, static_cast<int>(mouse_positions[i].x),
                                     static_cast<int>(mouse_positions[i].y));
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            },
            0x200, 0x4, SLEEP_MS);

        if (!offsets.empty()) {
            offset_registry.add("MouseService", "Position", offsets[0]);
        } else {
            LOG_ERR("Failed to get MousePosition offset, please make sure you don't move your "
                    "mouse when this phase runs");
        }

        move_mouse_in_window(roblox_window, client_width / 2, client_height / 2);

        return true;
    }

} // namespace scanner::phases