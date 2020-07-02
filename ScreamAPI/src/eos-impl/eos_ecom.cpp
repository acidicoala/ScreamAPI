#include "pch.h"
#include <ScreamAPI.h>

struct ClientDataContainer{
	void* originalClientData;
	EOS_Ecom_OnQueryOwnershipCallback originalCompletionDelegate;
};

EOS_CALL void ScreamAPIcompletionDelegate(const EOS_Ecom_QueryOwnershipCallbackInfo* Data){
	auto container = reinterpret_cast<ClientDataContainer*>(Data->ClientData);
	// get non-const pointer to data
	auto modifiedData = const_cast <EOS_Ecom_QueryOwnershipCallbackInfo*>(Data);

	// Restore original client data
	modifiedData->ClientData = container->originalClientData;

	Logger::dlc("Responding with %d item ownerships:", modifiedData->ItemOwnershipCount);

	// Modify ownership status of items.
	// This is where the magic happens ;)
	for(int i = 0; i < modifiedData->ItemOwnershipCount; i++){
		// Search the id in DLC list from the config
		bool isInOwnedList = std::find(Config::getOwnedItemIDs().begin(),
									   Config::getOwnedItemIDs().end(),
									   modifiedData->ItemOwnership[i].Id) != Config::getOwnedItemIDs().end();
		// Determine if it is owned
		bool owned = Config::ownAllDLC() || isInOwnedList;

		// Get non-const pointer to ownership struct
		auto itemOwnership = const_cast <EOS_Ecom_ItemOwnership*>(modifiedData->ItemOwnership + i);

		// Finally, change the ownership status
		itemOwnership->OwnershipStatus = owned ? EOS_EOwnershipStatus::EOS_OS_Owned : EOS_EOwnershipStatus::EOS_OS_NotOwned;

		auto ownershipStatusString = itemOwnership->OwnershipStatus == EOS_EOwnershipStatus::EOS_OS_Owned ? "Owned" : "NotOwned";
		Logger::dlc(" - Item ID: %s\t[%s]", itemOwnership->Id, ownershipStatusString);
	}

	// Call original completion delegated with our modified data
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

	auto container = new ClientDataContainer{ClientData, CompletionDelegate}; // Don't forget to delete
	static auto proxy = ScreamAPI::proxyFunction<decltype(EOS_Ecom_QueryOwnership)>(__FUNCTION__);
	proxy(Handle, Options, container, ScreamAPIcompletionDelegate);
}