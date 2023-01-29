#include <scream_api/config.hpp>

#include <koalabox/config.hpp>
#include <koalabox/logger.hpp>

namespace scream_api::config {

    Config instance; // NOLINT(cert-err58-cpp)

    void init() {
        instance = koalabox::config::parse<Config>();
    }

    DLL_EXPORT(void) ReloadConfig() {
        LOG_INFO("Reloading config")

        init();
    }
}
