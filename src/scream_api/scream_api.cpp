#include <build_config.h>
#include "scream_api.hpp"
#include "config/config.hpp"
#include "koalabox/logger/logger.hpp"
#include "koalabox/win_util/win_util.hpp"

namespace scream_api {
    using namespace koalabox;

    HMODULE original_module = nullptr;

    void init(HMODULE self_module) {
        DisableThreadLibraryCalls(self_module);

        koalabox::project_name = PROJECT_NAME;

        const auto self_directory = util::get_module_dir(self_module);

        config::init(self_directory / PROJECT_NAME".json");

        if (config::instance.logging) {
            logger::init(self_directory / PROJECT_NAME".log");
        }

        logger::info("🐨 {} v{}", PROJECT_NAME, PROJECT_VERSION);

        logger::debug("Initializing…");

        const auto original_module_path = self_directory / ORIG_DLL"_o.dll";

        original_module = win_util::load_library(original_module_path);

        logger::info("📚 Loaded original library from: '{}'", original_module_path.string());

        logger::info("🚀 Initialization complete");
    }

    void shutdown() {
        win_util::free_library(original_module);

        logger::info("💀 Shutdown complete");
    }

}
