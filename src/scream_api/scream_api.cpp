#include <build_config.h>
#include "scream_api.hpp"
#include "logger/logger.hpp"
#include "win_util/win_util.hpp"

void scream_api::init() {
    logger::info("🐨 {} v{}", PROJECT_NAME, PROJECT_VERSION);

    logger::debug("Initializing…");

    auto original_module_path = util::get_working_dir() / loader::get_original_module_name();

    if (not exists(original_module_path)) {
        util::panic(
            "Initialization error",
            "Original library not found at: {}"_format(original_module_path.string())
        );
    }

    win_util::load_library(original_module_path);

    logger::info("🚀 Initialization complete");
}

void scream_api::shutdown() {
    logger::debug("Shutting down…");

    auto original_module = loader::get_original_module_handle();

    win_util::free_library(original_module);

    logger::info("💀 Shutdown complete");
}