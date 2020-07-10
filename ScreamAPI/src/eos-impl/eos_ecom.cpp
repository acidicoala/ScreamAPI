#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"


/* Ownership */

struct ClientDataOwnershipContainer{
	void* originalClientData;
	EOS_Ecom_OnQueryOwnershipCallback originalCompletionDelegate;
};

void EOS_CALL queryOwnershipCompletionDelegate(const EOS_Ecom_QueryOwnershipCallbackInfo* Data){
	auto container = reinterpret_cast<ClientDataOwnershipContainer*>(Data->ClientData);

	// get non-const pointer to data
	auto modifiedData = const_cast <EOS_Ecom_QueryOwnershipCallbackInfo*>(Data);

	// Restore original client data
	modifiedData->ClientData = container->originalClientData;

	Logger::dlc("Responding with %d item ownerships:", modifiedData->ItemOwnershipCount);

	// Modify ownership status of items.
	// This is where the magic happens ;)
	for(unsigned int i = 0; i < modifiedData->ItemOwnershipCount; i++){
		// Search the id in DLC list from the config
		bool isInOwnedList = std::find(Config::getOwnedItemIDs().begin(),
									   Config::getOwnedItemIDs().end(),
									   modifiedData->ItemOwnership[i].Id) != Config::getOwnedItemIDs().end();

		// Get non-const pointer to ownership struct
		auto item = const_cast <EOS_Ecom_ItemOwnership*>(modifiedData->ItemOwnership + i);

		// Determine if this DLC should be unlocked
		bool unlocked = Config::isUnlockingAllDLC() || isInOwnedList;

		// Finally, change the ownership status
		item->OwnershipStatus = unlocked ? EOS_EOwnershipStatus::EOS_OS_Owned : EOS_EOwnershipStatus::EOS_OS_NotOwned;

		auto ownershipStatusString = item->OwnershipStatus == EOS_EOwnershipStatus::EOS_OS_Owned ? "Owned" : "Not Owned";
		Logger::dlc(" - Item ID: %s\t[%s]", item->Id, ownershipStatusString);
	}

	// Call original completion delegate with our modified data
	container->originalCompletionDelegate(modifiedData);

	// Free the heap
	delete container;
}

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnership(EOS_HEcom Handle, const EOS_Ecom_QueryOwnershipOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate){
	// Log item IDs
	if(Options){
		Logger::dlc("Game requested ownership of %d items:", Options->CatalogItemIdCount);
		for(uint32_t i = 0; i < Options->CatalogItemIdCount; i++)
			Logger::dlc(" - Item ID: %s", Options->CatalogItemIds[i]);
	} else{
		Logger::warn("Game requested DLC ownership, but without Options parameter");
	}

	if(Options)	// Verify SDK version
		ScreamAPI::checkSdkVersion(Options->ApiVersion, EOS_ECOM_QUERYOWNERSHIP_API_LATEST);

	auto container = new ClientDataOwnershipContainer{ClientData, CompletionDelegate}; // Don't forget to free the heap
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_QueryOwnership, __func__);
	proxy(Handle, Options, container, queryOwnershipCompletionDelegate);
}

/* Entitlements */

// TODO: Encapsulate these
int cachedEntitlementCount = 0;
std::vector<std::string> m_Entitlements;

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryEntitlements(EOS_HEcom Handle, const EOS_Ecom_QueryEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate){
	// Log item IDs
	if(Options){
		Logger::dlc("Game requested %d entitlements:", Options->EntitlementNameCount);
		for(uint32_t i = 0; i < Options->EntitlementNameCount; i++){
			//Logger::dlc(" - Entitlement Name: %s", Options->EntitlementNames[i]);
			m_Entitlements.emplace_back(Options->EntitlementNames[i]);
		}

		cachedEntitlementCount = Options->EntitlementNameCount;
	} else{
		Logger::warn("Game requested Entitlements, but without Options parameter");
	}

	if(Options)	// Verify SDK version
		ScreamAPI::checkSdkVersion(Options->ApiVersion, EOS_ECOM_QUERYENTITLEMENTS_API_LATEST);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_QueryEntitlements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Ecom_GetEntitlementsCount(EOS_HEcom Handle, const EOS_Ecom_GetEntitlementsCountOptions* Options){
	Logger::dlc("Responding with %d entitlements:", cachedEntitlementCount);
	return cachedEntitlementCount;
}
EOS_DECLARE_FUNC(EOS_EResult) EOS_Ecom_CopyEntitlementByIndex(EOS_HEcom Handle, const EOS_Ecom_CopyEntitlementByIndexOptions* Options, EOS_Ecom_Entitlement** OutEntitlement){
	size_t idLength = m_Entitlements.at(Options->EntitlementIndex).length();
	const char* idRaw = m_Entitlements.at(Options->EntitlementIndex).c_str();

	char* id = new char[idLength + 1];  // Don't forget to free the heap
	strcpy_s(id, idLength + 1, idRaw);

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
	//Logger::debug(" - EOS_Ecom_Entitlement_Release: %s", Entitlement->EntitlementId);

	// Free the heap
	delete Entitlement->EntitlementId;
	delete Entitlement;
}