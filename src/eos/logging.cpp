#include <sdk/eos_logging.h>
#include "config/config.hpp"
#include "scream_api/scream_api.hpp"

EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetLogLevel(
    EOS_ELogCategory LogCategory,
    EOS_ELogLevel LogLevel
) {
    static auto proxy = scream_api::get_original_function(&EOS_Logging_SetLogLevel, __func__);

    if (config::get().logging) {
        // Override game's preferences
        LogCategory = EOS_ELogCategory::EOS_LC_ALL_CATEGORIES;
        LogLevel = EOS_ELogLevel::EOS_LOG_VeryVerbose;
    }

    return proxy(LogCategory, LogLevel);
}

// Dummy implementation for use in ScreamAPI
EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetCallback(EOS_LogMessageFunc Callback) {
    static auto proxy = scream_api::get_original_function(&EOS_Logging_SetCallback, __func__);

    if (config::get().logging) {
        static std::set<EOS_LogMessageFunc> callbacks;

        callbacks.insert(Callback);

        return proxy(
            [](const EOS_LogMessage* Message) {
                for (const auto callback: callbacks) {
                    callback(Message);
                }
            }
        );
    } else {
        return proxy(Callback);
    }
}
