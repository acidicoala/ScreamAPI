#pragma once

namespace config {

    struct CatalogItems {
        bool unlock_all = true;
        std::set<std::string> override;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CatalogItems, unlock_all, override)
    };

    struct Entitlements {
        bool unlock_all = true;
        bool auto_inject = true;
        std::set<std::string> inject;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Entitlements, unlock_all, auto_inject, inject)
    };

    struct Config {
        bool logging = false;
        bool block_metrics = false;
        CatalogItems catalog_items;
        Entitlements entitlements;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, logging, block_metrics, catalog_items, entitlements)
    };

    const Config& get();

}
