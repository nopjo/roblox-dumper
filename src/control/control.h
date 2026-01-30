#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <nlohmann/json.hpp>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>

namespace control {
    using json = nlohmann::json;

    struct PendingRequest {
        std::string request_id;
        json response;
        bool completed = false;
        std::mutex mutex;
        std::condition_variable cv;
    };

    class Control {
      public:
        Control() = default;
        ~Control();

        auto start(int port = 8080) -> bool;
        auto stop() -> void;
        auto is_running() const -> bool { return m_running; }

        auto send_command(const json& command, int timeout_ms = 5000) -> std::optional<json>;

      private:
        auto generate_request_id() -> std::string;

        bool m_running = false;
        void* m_server = nullptr;
        std::unordered_map<std::string, std::shared_ptr<PendingRequest>> m_pending_requests;
        std::mutex m_requests_mutex;
        std::queue<json> m_command_queue;
        std::mutex m_command_mutex;
    };

    inline Control g_control;
} // namespace control