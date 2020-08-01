#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"
#include "util.h"

std::vector<std::string> entitlementIDs;

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryEntitlements(EOS_HEcom Handle, const EOS_Ecom_QueryEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate){
	// Log item IDs
	if(Options){
		// Verify SDK version
		ScreamAPI::checkSdkVersion(Options->ApiVersion, EOS_ECOM_QUERYENTITLEMENTS_API_LATEST);

		Logger::dlc("Game requested %d entitlements:", Options->EntitlementNameCount);
		for(uint32_t i = 0; i < Options->EntitlementNameCount; i++){
			Logger::dlc(" - Entitlement Name: %s", Options->EntitlementNames[i]);

			// Add to vector only if it isn't already there
			if(!Util::vectorContains<std::string>(entitlementIDs, Options->EntitlementNames[i]))
				entitlementIDs.emplace_back(Options->EntitlementNames[i]);
		}
	} else{
		Logger::warn("Game requested Entitlements, but without Options parameter");
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_QueryEntitlements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetEntitlementsCount(EOS_HEcom Handle, const EOS_Ecom_GetEntitlementsCountOptions* Options){
	if(Options){
		ScreamAPI::checkSdkVersion(Options->ApiVersion, EOS_ECOM_GETENTITLEMENTSCOUNT_API_LATEST);
	} else{
		Logger::warn("Game requested Entitlement Count, but without Options parameter");
	}

	uint32_t entitlementCount;
	if(Config::isProxyingEntitlements()){
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_GetEntitlementsCount, __func__);
		entitlementCount = proxy(Handle, Options);
	} else{
		entitlementCount = entitlementIDs.size();
	}

	Logger::dlc("Responding with %d entitlements:", entitlementCount);
	return entitlementCount;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement){
	if(Config::isProxyingEntitlements()){
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_CopyEntitlementByIndex, __func__);
		auto result = proxy(Handle, Options, OutEntitlement);
		Logger::dlc("Proxy CopyEntitlementByIndex:");
		Logger::dlc(" - EntitlementId: %s", (*OutEntitlement)->EntitlementId);
		Logger::dlc(" - CatalogItemId: %s", (*OutEntitlement)->CatalogItemId);
		Logger::dlc(" - EntitlementName: %s", (*OutEntitlement)->EntitlementName);
		Logger::dlc(" - bRedeemed: %d", (*OutEntitlement)->bRedeemed);
		Logger::dlc(" - ServerIndex: %d", (*OutEntitlement)->ServerIndex);
		Logger::dlc(" - EndTimestamp: %ll", (*OutEntitlement)->EndTimestamp);
		return result;
	}

	if(Options){
		ScreamAPI::checkSdkVersion(Options->ApiVersion, EOS_ECOM_COPYENTITLEMENTBYINDEX_API_LATEST);
	} else{
		Logger::warn("Game requested CopyEntitlementByIndex, but without Options parameter");
		return EOS_EResult::EOS_InvalidParameters;
	}

	size_t idLength = entitlementIDs.at(Options->EntitlementIndex).length() + 1;
	const char* idRaw = entitlementIDs.at(Options->EntitlementIndex).c_str();

	char* id = new char[idLength];  // Don't forget to free the heap
	strcpy_s(id, idLength, idRaw);

	auto entitlement = new EOS_Ecom_Entitlement(); // Don't forget to free the heap
	entitlement->ApiVersion = EOS_ECOM_ENTITLEMENT_API_LATEST;
	entitlement->EntitlementId = id;
	entitlement->CatalogItemId = id;
	entitlement->EntitlementName = id;
	entitlement->bRedeemed = false;
	entitlement->EndTimestamp = -1;
	entitlement->ServerIndex = -1;

	Logger::dlc(" - Entitlement ID: %s", entitlement->EntitlementId);

	*OutEntitlement = entitlement;
	return EOS_EResult::EOS_Success;
}

EOS_DECLARE_FUNC(void) EOS_Ecom_Entitlement_Release(EOS_Ecom_Entitlement* Entitlement){
	Logger::debug(" - EOS_Ecom_Entitlement_Release: %s", Entitlement->EntitlementId);

	if(Config::isProxyingEntitlements()){
		static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_Entitlement_Release, __func__);
		proxy(Entitlement);
		return;
	}

	// Free the heap
	delete Entitlement->EntitlementId;
	delete Entitlement;
}
