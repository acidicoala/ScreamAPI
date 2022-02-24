#pragma once

#include <sdk/eos_base.h>

#define DLL_EXPORT(TYPE) EOS_DECLARE_FUNC(TYPE)

#define GET_ORIGINAL_FUNCTION(FUNC) \
    static const auto FUNC##_o = hook::get_original_function( \
        scream_api::is_hook_mode, \
        scream_api::original_library, \
        loader::get_decorated_function(scream_api::original_library, #FUNC), \
        FUNC \
    );

namespace scream_api {
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
        bool eos_logging = false;
        bool block_metrics = false;
        CatalogItems catalog_items;
        Entitlements entitlements;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config,
            logging, eos_logging, block_metrics, catalog_items, entitlements
        )
    };

    extern Config config;

    extern String namespace_id;

    extern HMODULE original_library;

    extern bool is_hook_mode;

    void init(HMODULE self_module);

    void shutdown();

}
