#include <sdk/eos_common.h>
#include "scream_api/scream_api.hpp"

EOS_DECLARE_FUNC(const char*) EOS_EResult_ToString(EOS_EResult Result) {
    static auto proxy = scream_api::get_original_function(&EOS_EResult_ToString, __func__);
    return proxy(Result);
}
