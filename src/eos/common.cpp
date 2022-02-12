#include <sdk/eos_common.h>
#include "scream_api/scream_api.hpp"

DLL_EXPORT(const char*) EOS_EResult_ToString(EOS_EResult Result) {
    static auto EOS_EResult_ToString_o = scream_api::get_original_function(
        &EOS_EResult_ToString,
        __func__
    );

    return EOS_EResult_ToString_o(Result);
}
