#pragma once

#include "std_ext.hpp"

namespace config {

    struct CatalogItems {
        bool unlock_all = true;
        std_ext::list<std::string> override;
    };

    struct Entitlements {
        bool unlock_all = true;
        bool auto_inject = true;
        std_ext::list<std::string> inject;
    };

    struct Config {
        bool logging = false;
        bool block_metrics = false;
        CatalogItems catalog_items;
        Entitlements entitlements;
    };

    const Config& get();
}
