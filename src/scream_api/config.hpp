#pragma once

#include <koalabox/core.hpp>
#include <scream_api/scream_api.hpp>

namespace scream_api::config {

    struct CatalogItems {
        bool unlock_all = true;
        Set<String> override;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            CatalogItems,
            unlock_all,
            override
        )
    };

    struct Entitlements {
        bool unlock_all = true;
        bool auto_inject = true;
        Set<String> inject;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            Entitlements,
            unlock_all,
            auto_inject,
            inject
        )
    };

    struct Config {
        bool logging = false;
        bool eos_logging = false;
        bool block_metrics = false;
        CatalogItems catalog_items;
        Entitlements entitlements;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            Config,
            logging,
            eos_logging,
            block_metrics,
            catalog_items,
            entitlements
        )
    };

    extern Config instance;

    void init();

    DLL_EXPORT(void) ReloadConfig();

}
