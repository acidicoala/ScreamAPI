#include <sdk/eos_logging.h>
#include "config/config.hpp"
#include "scream_api/scream_api.hpp"

DLL_EXPORT(EOS_EResult) EOS_Logging_SetLogLevel(
    EOS_ELogCategory LogCategory,
    EOS_ELogLevel LogLevel
) {
    static const auto EOS_Logging_SetLogLevel_o = scream_api::get_original_function(
        &EOS_Logging_SetLogLevel,
        __func__
    );

    if (config::instance.logging) {
        // Override game's preferences
        LogCategory = EOS_ELogCategory::EOS_LC_ALL_CATEGORIES;
        LogLevel = EOS_ELogLevel::EOS_LOG_VeryVerbose;
    }

    return EOS_Logging_SetLogLevel_o(LogCategory, LogLevel);
}

// Dummy implementation for use in ScreamAPI
DLL_EXPORT(EOS_EResult) EOS_Logging_SetCallback(EOS_LogMessageFunc Callback) {
    static const auto EOS_Logging_SetCallback_o = scream_api::get_original_function(
        &EOS_Logging_SetCallback,
        __func__
    );

    if (config::instance.logging) {
        static std::set<EOS_LogMessageFunc> callbacks;

        callbacks.insert(Callback);

        return EOS_Logging_SetCallback_o(
            [](const EOS_LogMessage* Message) {
                for (const auto callback: callbacks) {
                    callback(Message);
                }
            }
        );
    } else {
        return EOS_Logging_SetCallback_o(Callback);
    }
}
