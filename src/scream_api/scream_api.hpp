#pragma once

#include <sdk/eos_base.h>
#include "koalabox/loader/loader.hpp"
#include "koalabox/util/util.hpp"
#include "koalabox/win_util/win_util.hpp"

#define DLL_EXPORT(TYPE) EOS_DECLARE_FUNC(TYPE)

#define GET_ORIGINAL_FUNCTION(FUNC)\
    static const auto FUNC##_o = koalabox::loader::get_original_function(\
        scream_api::is_hook_mode,\
        scream_api::original_library,\
        koalabox::loader::get_undecorated_function(scream_api::original_library, #FUNC),\
        FUNC\
    );\

namespace scream_api {
    using namespace koalabox;

    extern String namespace_id;

    extern HMODULE original_library;

    extern bool is_hook_mode;

    void init(HMODULE self_module);

    void shutdown();

}
