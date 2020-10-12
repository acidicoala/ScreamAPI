#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"
#include "util.h"


#define TRANSACTION_UNLOCKER false

#if TRANSACTION_UNLOCKER
EOS_Ecom_HTransaction ScreamAPITransaction;
extern std::vector<std::string> entitlementIDs;
constexpr auto ScreamAPITransactionId = "12345678901234567890123456789012";
#endif


// Just a snooper function at this point to debug transactions.
// Might enable transaction spoofing later on,
// if there are appropriate games to test it on.
EOS_DECLARE_FUNC(void) EOS_Ecom_Checkout(EOS_HEcom Handle, const EOS_Ecom_CheckoutOptions* Options, void* ClientData, const EOS_Ecom_OnCheckoutCallback CompletionDelegate){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Checkout, __func__);
	auto container = new ScreamAPI::OriginalDataContainer(ClientData, CompletionDelegate);
	proxy(Handle, Options, container, [](const EOS_Ecom_CheckoutCallbackInfo* Data){
		ScreamAPI::proxyCallback<EOS_Ecom_CheckoutCallbackInfo>(Data, &Data->ClientData,
			[](EOS_Ecom_CheckoutCallbackInfo* mData){
#if TRANSACTION_UNLOCKER
			mData->TransactionId = ScreamAPITransactionId;
			mData->ResultCode = EOS_EResult::EOS_Success;
#endif
			Logger::dlc("\t""ResultString: %s", EOS_EResult_ToString(mData->ResultCode));
			Logger::dlc("\t""TransactionId: %s", mData->TransactionId);
		});
	});
}

#if TRANSACTION_UNLOCKER

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyTransactionById(EOS_HEcom Handle, const EOS_Ecom_CopyTransactionByIdOptions* Options, EOS_Ecom_HTransaction* OutTransaction){
	Logger::debug(__func__);

	Logger::dlc("\t""TransactionId: %s", Options->TransactionId);
	if(!strcmp(Options->TransactionId, ScreamAPITransactionId)){
		Logger::debug("\t""Responding with ScreamAPITransaction");
		*OutTransaction = ScreamAPITransaction;
		return EOS_EResult::EOS_Success;
	}
	else {
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_CopyTransactionById, __func__);
		return proxy(Handle, Options, OutTransaction);
	}
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyTransactionByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyTransactionByIndexOptions* Options, EOS_Ecom_HTransaction* OutTransaction){
	Logger::debug(__func__);

	Logger::dlc("\t""TransactionIndex: %d", Options->TransactionIndex);
	if(true){ // TODO: Replace with config option later
		Logger::debug("\t""Responding with ScreamAPITransaction");
		*OutTransaction = ScreamAPITransaction;
		return EOS_EResult::EOS_Success;
	}
	else {
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_CopyTransactionByIndex, __func__);
		return proxy(Handle, Options, OutTransaction);
	}
}

EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetTransactionCount(EOS_HEcom Handle, const EOS_Ecom_GetTransactionCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_GetTransactionCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::dlc("\t""Original TransactionCount: %d", result);
	return 1;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_Transaction_GetTransactionId(EOS_Ecom_HTransaction Handle, char* OutBuffer, int32_t* InOutBufferLength){
	Logger::debug(__func__);

	if(Handle == ScreamAPITransaction){
		Logger::dlc("Responding with ScreamAPI TransactionId: %s", ScreamAPITransactionId);
		OutBuffer = Util::copy_c_string(ScreamAPITransactionId);
		*InOutBufferLength = 32;
		return EOS_EResult::EOS_Success;
	}
	else {
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Transaction_GetTransactionId, __func__);
		return proxy(Handle, OutBuffer, InOutBufferLength);
	}
}

EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_Transaction_GetEntitlementsCount(EOS_Ecom_HTransaction Handle, const EOS_Ecom_Transaction_GetEntitlementsCountOptions* Options){
	Logger::debug(__func__);

	if(Handle == ScreamAPITransaction){
		Logger::dlc("Responding with GetEntitlementsCount: %d", entitlementIDs.size());
		return (uint32_t)entitlementIDs.size();
	}
	else {
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Transaction_GetEntitlementsCount, __func__);
		return proxy(Handle, Options);
	}
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_Transaction_CopyEntitlementByIndex(EOS_Ecom_HTransaction Handle, const EOS_Ecom_Transaction_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement){
	Logger::debug(__func__);

	if(Handle == ScreamAPITransaction){
		// TODO: extract it into a function, since it is also used by entitlements
		const char* id = Util::copy_c_string(entitlementIDs.at(Options->EntitlementIndex).c_str());

		auto entitlement = new EOS_Ecom_Entitlement(); // Don't forget to free the heap
		entitlement->ApiVersion = EOS_ECOM_ENTITLEMENT_API_LATEST;
		entitlement->EntitlementId = id;
		entitlement->CatalogItemId = id;
		entitlement->EntitlementName = id;
		entitlement->bRedeemed = false;
		entitlement->EndTimestamp = -1;
		entitlement->ServerIndex = -1;

		Logger::dlc("\t""Entitlement ID: %s", entitlement->EntitlementId);

		*OutEntitlement = entitlement;
		return EOS_EResult::EOS_Success;
	}
	else {
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Transaction_CopyEntitlementByIndex, __func__);
		return proxy(Handle, Options, OutEntitlement);
	}
}

EOS_DECLARE_FUNC(void) EOS_Ecom_Transaction_Release(EOS_Ecom_HTransaction Transaction){
	Logger::debug(__func__);

	if(Transaction == ScreamAPITransaction){
		Logger::dlc("Pretending to release the transaction handle...");
	}
	else {
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Transaction_Release, __func__);
		proxy(Transaction);
	}
}

#endif
