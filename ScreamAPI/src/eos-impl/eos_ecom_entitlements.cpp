#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"
#include "util.h"

std::vector<std::string> entitlementIDs;
bool useLegitEntitlements = true;

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryEntitlements(EOS_HEcom Handle, const EOS_Ecom_QueryEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate){
	Logger::debug(__func__);

	Logger::dlc("Game requested %d entitlements:", Options->EntitlementNameCount);
	for(uint32_t i = 0; i < Options->EntitlementNameCount; i++){
		Logger::dlc("\t""Entitlement Name: %s", Options->EntitlementNames[i]);

		// Add to vector only if it isn't already there
		if(!Util::vectorContains(entitlementIDs, std::string(Options->EntitlementNames[i])))
			entitlementIDs.emplace_back(Options->EntitlementNames[i]);
	}
	useLegitEntitlements = false;

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_QueryEntitlements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetEntitlementsCount(EOS_HEcom Handle, const EOS_Ecom_GetEntitlementsCountOptions* Options){
	Logger::debug(__func__);

	uint32_t entitlementCount;
	if(useLegitEntitlements || !Config::EnableEntitlementUnlocker()){
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_GetEntitlementsCount, __func__);
		entitlementCount = proxy(Handle, Options);
	} else{
		entitlementCount = (uint32_t) entitlementIDs.size();
	}

	Logger::dlc("Responding with %d entitlements:", entitlementCount);
	return entitlementCount;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement){
	Logger::debug(__func__);

	if(Config::EnableEntitlementUnlocker()){
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
	} else{
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_CopyEntitlementByIndex, __func__);
		auto result = proxy(Handle, Options, OutEntitlement);
		Logger::dlc("Proxy CopyEntitlementByIndex:");
		Logger::dlc("\t""EntitlementId: %s", (*OutEntitlement)->EntitlementId);
		Logger::dlc("\t""CatalogItemId: %s", (*OutEntitlement)->CatalogItemId);
		Logger::dlc("\t""EntitlementName: %s", (*OutEntitlement)->EntitlementName);
		Logger::dlc("\t""bRedeemed: %d", (*OutEntitlement)->bRedeemed);
		Logger::dlc("\t""ServerIndex: %d", (*OutEntitlement)->ServerIndex);
		Logger::dlc("\t""EndTimestamp: %ll", (*OutEntitlement)->EndTimestamp);
		return result;
	}
}

EOS_DECLARE_FUNC(void) EOS_Ecom_Entitlement_Release(EOS_Ecom_Entitlement* Entitlement){
	//Logger::debug(__func__);

	if(Config::EnableEntitlementUnlocker()){
		// Free the heap
		delete Entitlement->EntitlementId;
		delete Entitlement;
	} else{
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Entitlement_Release, __func__);
		proxy(Entitlement);
	}
}
