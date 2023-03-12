#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <store_mode/store_mode.hpp>
#include <game_mode/game_mode.hpp>
#include <build_config.h>

#include <koalabox/config.hpp>
#include <koalabox/dll_monitor.hpp>
#include <koalabox/globals.hpp>
#include <koalabox/win_util.hpp>

namespace scream_api {

    bool is_store_mode = false;

    bool is_epic_games_launcher(const String& exe_name) {
        return exe_name < equals > "EpicGamesLauncher.exe";
    }

    void init(HMODULE self_handle) {
        try {
            koalabox::globals::init_globals(self_handle, PROJECT_NAME);

            config::init();

            if (config::instance.logging) {
                koalabox::logger::init_file_logger();
            }

            LOG_INFO("🐨 {} v{} | Compiled at '{}'", PROJECT_NAME, PROJECT_VERSION, __TIMESTAMP__)

            const auto exe_path = Path(koalabox::win_util::get_module_file_name_or_throw(nullptr));
            const auto exe_name = exe_path.filename().string();

            LOG_DEBUG("Process name: '{}' [{}-bit]", exe_name, BITNESS)

            if (is_epic_games_launcher(exe_name)) {
                is_store_mode = true;
                store_mode::init_store_mode(exe_path);
            } else {
                game_mode::init_game_mode(self_handle);
            }

            LOG_INFO("🚀 Initialization complete")
        } catch (const Exception& e) {
            koalabox::util::panic("Initialization error: {}", e.what());
        }
    }

    void shutdown() {
        LOG_INFO("💀 Shutdown complete")

        if(is_store_mode){
            store_mode::shutdown_store_mode();
        }
    }

}
