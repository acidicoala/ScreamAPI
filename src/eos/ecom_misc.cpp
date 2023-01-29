#include <scream_api/scream_api.hpp>
#include <scream_api/config.hpp>
#include <game_mode/game_mode.hpp>

#include <koalabox/logger.hpp>

#include <sdk/eos_ecom.h>

using namespace koalabox;

/// This file contains stub implementation of functions
/// for the purposes of logging and potential future improvements

#define IMPLEMENTATION(FUNC, ...) \
    GET_ORIGINAL_FUNCTION(FUNC) \
    LOG_DEBUG("Function called: {}", #FUNC); \
    return FUNC##_o(__VA_ARGS__);

DLL_EXPORT(void) EOS_Ecom_QueryOwnershipToken(
    EOS_HEcom Handle,
    const EOS_Ecom_QueryOwnershipTokenOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnQueryOwnershipTokenCallback CompletionDelegate
) {
    IMPLEMENTATION(EOS_Ecom_QueryOwnershipToken, Handle, Options, ClientData, CompletionDelegate)
}

DLL_EXPORT(void) EOS_Ecom_RedeemEntitlements(
    EOS_HEcom Handle,
    const EOS_Ecom_RedeemEntitlementsOptions* Options,
    void* ClientData,
    const EOS_Ecom_OnRedeemEntitlementsCallback CompletionDelegate
) {
    IMPLEMENTATION(EOS_Ecom_RedeemEntitlements, Handle, Options, ClientData, CompletionDelegate)
}

DLL_EXPORT(uint32_t) EOS_Ecom_GetEntitlementsByNameCount(
    EOS_HEcom Handle,
    const EOS_Ecom_GetEntitlementsByNameCountOptions* Options
) {
    IMPLEMENTATION(EOS_Ecom_GetEntitlementsByNameCount, Handle, Options)
}

DLL_EXPORT(EOS_EResult) EOS_Ecom_CopyEntitlementByNameAndIndex(
    EOS_HEcom Handle,
    const EOS_Ecom_CopyEntitlementByNameAndIndexOptions* Options,
    EOS_Ecom_Entitlement** OutEntitlement
) {
    IMPLEMENTATION(EOS_Ecom_CopyEntitlementByNameAndIndex, Handle, Options, OutEntitlement)
}

DLL_EXPORT(EOS_EResult) EOS_Ecom_CopyItemById(
    EOS_HEcom Handle,
    const EOS_Ecom_CopyItemByIdOptions* Options,
    EOS_Ecom_CatalogItem** OutItem
) {
    IMPLEMENTATION(EOS_Ecom_CopyItemById, Handle, Options, OutItem)
}

DLL_EXPORT(uint32_t) EOS_Ecom_GetItemReleaseCount(
    EOS_HEcom Handle,
    const EOS_Ecom_GetItemReleaseCountOptions* Options
) {
    IMPLEMENTATION(EOS_Ecom_GetItemReleaseCount, Handle, Options)
}

// This function is used in ScreamAPI

DLL_EXPORT(const char*) EOS_EResult_ToString(EOS_EResult Result) {
    IMPLEMENTATION(EOS_EResult_ToString, Result)
}
