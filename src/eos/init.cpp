#include <sdk/eos_init.h>
#include <sdk/eos_logging.h>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "scream_api/scream_api.hpp"

std::string scream_api::namespace_id;

EOS_DECLARE_FUNC(EOS_HPlatform) EOS_Platform_Create(const EOS_Platform_Options* Options) {
    static auto proxy = scream_api::get_original_function(&EOS_Platform_Create, __func__);

    if (Options != nullptr) {
        logger::info("🗃 DLC database: https://scream-db.web.app/offers/{}", Options->SandboxId);
        scream_api::namespace_id = Options->SandboxId;
    }

    auto result = proxy(Options);

    if (config::get().logging) {
        // Enable EOS logging
        EOS_Logging_SetCallback(
            [](const EOS_LogMessage* Message) {
                logger::debug("[{}]\t{}", Message->Category, Message->Message);
            }
        );

        EOS_Logging_SetLogLevel(
            EOS_ELogCategory::EOS_LC_ALL_CATEGORIES,
            EOS_ELogLevel::EOS_LOG_VeryVerbose
        );
    }

    return result;
}
