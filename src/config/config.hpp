#pragma once

#include "koalabox/koalabox.hpp"

namespace config {

    using namespace koalabox;

    struct CatalogItems {
        bool unlock_all = true;
        Set<String> override;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CatalogItems, unlock_all, override)
    };

    struct Entitlements {
        bool unlock_all = true;
        bool auto_inject = true;
        Set<String> inject;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Entitlements, unlock_all, auto_inject, inject)
    };

    struct Config {
        bool logging = false;
        bool block_metrics = false;
        CatalogItems catalog_items;
        Entitlements entitlements;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, logging, block_metrics, catalog_items, entitlements)
    };

    extern Config instance;

    void init(Path path);

}
