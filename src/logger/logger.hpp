#pragma once

namespace logger {

    std::shared_ptr<spdlog::logger> instance();

    template<typename... Args>
    void trace(fmt::format_string<Args...> fmt, Args&& ...args) {
        instance()->trace(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(fmt::format_string<Args...> fmt, Args&& ...args) {
        instance()->debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&& ...args) {
        instance()->info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args&& ...args) {
        instance()->warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&& ...args) {
        instance()->error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&& ...args) {
        instance()->critical(fmt, std::forward<Args>(args)...);
    }

}
