#pragma once

#include <koalabox/hook.hpp>
#include <koalabox/loader.hpp>

#define GET_ORIGINAL_FUNCTION(FUNC) \
    static const auto $func_name = koalabox::loader::get_decorated_function( \
        scream_api::game_mode::original_library, \
        #FUNC \
    ); \
    static const auto FUNC##_o = scream_api::game_mode::is_hook_mode \
        ? koalabox::hook::get_original_hooked_function($func_name, FUNC) \
        : koalabox::hook::get_original_function( \
            scream_api::game_mode::original_library, \
            $func_name, \
            FUNC \
        );

namespace scream_api::game_mode {

    extern String namespace_id;

    extern HMODULE original_library;

    extern bool is_hook_mode;

    void init_game_mode(HMODULE self_handle);

}
