#include <sdk/eos_common.h>
#include "scream_api/scream_api.hpp"
#include "logger/logger.hpp"

EOS_DECLARE_FUNC(EOS_EResult) EOS_EpicAccountId_ToString(
    EOS_EpicAccountId AccountId,
    char* OutBuffer,
    int32_t* InOutBufferLength
) {
    static auto proxy = scream_api::get_original_function(&EOS_EpicAccountId_ToString, __func__);
    return proxy(AccountId, OutBuffer, InOutBufferLength);
}

EOS_DECLARE_FUNC(EOS_EpicAccountId) EOS_EpicAccountId_FromString(
    const char* AccountIdString
) {
    static auto proxy = scream_api::get_original_function(&EOS_EpicAccountId_FromString, __func__);
    return proxy(AccountIdString);
}
