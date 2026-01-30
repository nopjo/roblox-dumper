#include "logger.h"
#include <mutex>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

namespace logger {

    template <typename Mutex> class error_cache_sink : public spdlog::sinks::base_sink<Mutex> {
      public:
        auto get_errors() const -> std::vector<std::string> {
            std::lock_guard<Mutex> lock(
                const_cast<Mutex&>(spdlog::sinks::base_sink<Mutex>::mutex_));
            return error_messages_;
        }

      protected:
        void sink_it_(const spdlog::details::log_msg& msg) override {
            if (msg.level >= spdlog::level::err) {
                spdlog::memory_buf_t formatted;
                spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
                error_messages_.emplace_back(formatted.data(), formatted.size());
            }
        }

        void flush_() override {}

      private:
        mutable std::vector<std::string> error_messages_;
    };

    using error_cache_sink_mt = error_cache_sink<std::mutex>;

    inline std::shared_ptr<error_cache_sink_mt> g_error_sink;

    auto initialize() -> void {
        std::vector<spdlog::sink_ptr> sinks;

        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("roblox-dumper.log", true));

        // credits for a random old thread i found on the error sink

        g_error_sink = std::make_shared<error_cache_sink_mt>();
        sinks.push_back(g_error_sink);

        auto logger = std::make_shared<spdlog::logger>("roblox_dumper", sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::trace);
        logger->set_pattern("[%T] [%^%l%$] %v");

        spdlog::set_default_logger(logger);
    }

    auto print_error_summary() -> void {
        if (!g_error_sink)
            return;

        const auto errors = g_error_sink->get_errors();
        if (errors.empty()) {
            spdlog::info("No errors encountered!");
            return;
        }

        spdlog::warn("\n{} Error(s) Encountered", errors.size());
        for (const auto& error : errors) {
            spdlog::warn("{}", error);
        }
    }
} // namespace logger