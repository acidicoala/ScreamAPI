#pragma once

namespace logger {

    std::shared_ptr<spdlog::logger> instance();

    template<typename FormatString, typename... Args>
    void trace(const FormatString& fmt, Args&& ... args) {
        instance()->trace(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    void debug(const FormatString& fmt, Args&& ... args) {
        instance()->debug(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    void info(const FormatString& fmt, Args&& ... args) {
        instance()->info(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    void warn(const FormatString& fmt, Args&& ... args) {
        instance()->warn(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    void error(const FormatString& fmt, Args&& ...args) {
        instance()->error(fmt, std::forward<Args>(args)...);
    }

    template<typename FormatString, typename... Args>
    void critical(const FormatString& fmt, Args&& ... args) {
        instance()->critical(fmt, std::forward<Args>(args)...);
    }

}
