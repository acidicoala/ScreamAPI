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

    bool is_dlc_unlocked(String game_id, String dlc_id, bool original_unlocked) {
        auto status = instance.default_game_status;

        if (instance.override_game_status.contains(game_id)) {
            status = instance.override_game_status[game_id];
        }

        if (instance.override_dlc_status.contains(dlc_id)) {
            status = instance.override_dlc_status[dlc_id];
        }

        bool is_unlocked;
        switch (status) {
            case ItemStatus::UNLOCKED:
                is_unlocked = true;
                break;
            case ItemStatus::LOCKED:
                is_unlocked = false;
                break;
            case ItemStatus::ORIGINAL:
            case ItemStatus::UNDEFINED:
                is_unlocked = original_unlocked;
                break;
        }

        LOG_TRACE(
            "Game ID: {}, DLC ID: {}, Status: {}, Original: {}, Unlocked: {}",
            game_id, dlc_id, Json(status).dump(), original_unlocked, is_unlocked
        )

        return is_unlocked;
    }
}
