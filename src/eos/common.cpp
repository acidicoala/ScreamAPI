#include <sdk/eos_common.h>
#include "scream_api/scream_api.hpp"

DLL_EXPORT(const char*) EOS_EResult_ToString(EOS_EResult Result) {
    GET_ORIGINAL_FUNCTION(EOS_EResult_ToString)

    return EOS_EResult_ToString_o(Result);
}
