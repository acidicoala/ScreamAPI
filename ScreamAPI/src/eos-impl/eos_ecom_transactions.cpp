#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"

struct CheckoutContainer{
	void* originalClientData;
	EOS_Ecom_OnCheckoutCallback originalCompletionDelegate;
};

// Just a snooper function at this point to debug transactions.
// Might implement a transaction spoofing later on,
// if there are appropriate games to test it on.
void EOS_CALL CheckoutCompletionDelegate(const EOS_Ecom_CheckoutCallbackInfo* Data){
	auto container = reinterpret_cast<CheckoutContainer*>(Data->ClientData);

	// get non-const pointer to data
	auto modifiedData = const_cast<EOS_Ecom_CheckoutCallbackInfo*>(Data);

	// Restore original client data
	modifiedData->ClientData = container->originalClientData;

	Logger::debug("CheckoutCompletionDelegate:");
	Logger::debug(" - ResultCode: %d", Data->ResultCode);
	Logger::debug(" - TransactionId: %s", Data->TransactionId);

	container->originalCompletionDelegate(Data);
}

EOS_DECLARE_FUNC(void) EOS_Ecom_Checkout(EOS_HEcom Handle, const EOS_Ecom_CheckoutOptions* Options, void* ClientData, const EOS_Ecom_OnCheckoutCallback CompletionDelegate){
	Logger::debug(__func__);

	auto container = new CheckoutContainer{ClientData, CompletionDelegate}; // Don't forget to free the heap
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Checkout, __func__);
	proxy(Handle, Options, container, CheckoutCompletionDelegate);
}

// EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetTransactionCount(EOS_HEcom Handle, const EOS_Ecom_GetTransactionCountOptions* Options);
// EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyTransactionByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyTransactionByIndexOptions* Options, EOS_Ecom_HTransaction* OutTransaction);
// EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyTransactionById(EOS_HEcom Handle, const EOS_Ecom_CopyTransactionByIdOptions* Options, EOS_Ecom_HTransaction* OutTransaction);
// EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_Transaction_GetTransactionId(EOS_Ecom_HTransaction Handle, char* OutBuffer, int32_t* InOutBufferLength);
// EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_Transaction_GetEntitlementsCount(EOS_Ecom_HTransaction Handle, const EOS_Ecom_Transaction_GetEntitlementsCountOptions* Options);
// EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_Transaction_CopyEntitlementByIndex(EOS_Ecom_HTransaction Handle, const EOS_Ecom_Transaction_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement);
// EOS_DECLARE_FUNC(void) EOS_Ecom_Transaction_Release(EOS_Ecom_HTransaction Transaction);