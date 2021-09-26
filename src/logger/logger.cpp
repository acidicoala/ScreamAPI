#include <build_config.h>
#include "logger.hpp"
#include "config/config.hpp"
#include "util/util.hpp"

class EmojiFormatterFlag : public spdlog::custom_flag_formatter {
public:
    void format(
        const spdlog::details::log_msg& log_msg,
        const std::tm&,
        spdlog::memory_buf_t& dest
    ) override {
        std::string some_txt;
        switch (log_msg.level) {
            case spdlog::level::critical:
                some_txt = "💥";
                break;
            case spdlog::level::err:
                some_txt = "❌";
                break;
            case spdlog::level::warn:
                some_txt = "⚠";
                break;
            case spdlog::level::info:
                some_txt = "ℹ";
                break;
            case spdlog::level::debug:
                some_txt = "⬛";
                break;
            case spdlog::level::trace:
                some_txt = "🔍";
                break;
            default:
                some_txt = " ";
                break;
        }

        dest.append(some_txt.data(), some_txt.data() + some_txt.size());
    }

    std::unique_ptr<custom_flag_formatter> clone() const override {
        return spdlog::details::make_unique<EmojiFormatterFlag>();
    }
};

std::shared_ptr<spdlog::logger> logger::instance() {
    static auto logger = [] {

        if (config::get().logging) {
            auto path = util::get_working_dir() / PROJECT_NAME".log";
            auto logger = spdlog::basic_logger_mt("default", path.string(), true);

            auto formatter = std::make_unique<spdlog::pattern_formatter>();
            formatter->add_flag<EmojiFormatterFlag>('*').set_pattern("[%H:%M:%S.%e] %* ┃ %v");

            logger->set_formatter(std::move(formatter));
            logger->set_level(spdlog::level::trace);
            logger->flush_on(spdlog::level::debug);

            return logger;
        } else {
            return spdlog::null_logger_mt("null");
        }
    }();

    return logger;
}
