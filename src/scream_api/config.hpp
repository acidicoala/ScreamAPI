#pragma once

#include <koalabox/core.hpp>
#include <scream_api/scream_api.hpp>

namespace scream_api::config {

    struct MitmProxy {
        int listen_port = 9999;
        String extra_args = "";
        Map<String, String> upstream_proxies;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(MitmProxy, listen_port, extra_args, upstream_proxies);
    };

    struct Game {
        // Key is namespace, value is item name
        Map<String, String> entitlements;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Game, entitlements);
    };

    using GameEntitlementsMap = Map<String, Game>;

    enum class ItemStatus {
        UNDEFINED,
        ORIGINAL,
        UNLOCKED,
        LOCKED,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(ItemStatus, {
        { ItemStatus::UNDEFINED, nullptr },
        { ItemStatus::ORIGINAL, "original" },
        { ItemStatus::UNLOCKED, "unlocked" },
        { ItemStatus::LOCKED, "locked" },
    })

    struct Config {
        int $version = 3;
        bool logging = false;
        bool eos_logging = false;
        bool block_metrics = false;
        ItemStatus default_game_status = ItemStatus::UNLOCKED;
        Map<String, ItemStatus> override_game_status;
        Map<String, ItemStatus> override_dlc_status;
        GameEntitlementsMap extra_entitlements;
        MitmProxy mitmproxy;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            Config,
            logging,
            eos_logging,
            block_metrics,
            default_game_status,
            override_game_status,
            override_dlc_status,
            extra_entitlements,
            mitmproxy
        )
    };

    extern Config instance;

    void init();

    DLL_EXPORT(void) ReloadConfig();

    bool is_dlc_unlocked(String game_id, String dlc_id, bool original_unlocked);

}
