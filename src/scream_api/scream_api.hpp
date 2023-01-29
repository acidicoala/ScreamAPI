#pragma once

#include <koalabox/core.hpp>

#define EOS_BUILDING_SDK TRUE
#include <sdk/eos_base.h>

#define DLL_EXPORT(TYPE) EOS_DECLARE_FUNC(TYPE)

namespace scream_api {

    void init(HMODULE self_handle);

    void shutdown();

}
