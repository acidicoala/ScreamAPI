#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"

struct ClientDataContainer{
	void* originalClientData;
	EOS_Ecom_OnQueryOwnershipCallback originalCompletionDelegate;
};

void EOS_CALL ScreamAPIcompletionDelegate(const EOS_Ecom_QueryOwnershipCallbackInfo* Data){
	auto container = reinterpret_cast<ClientDataContainer*>(Data->ClientData);

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

	auto container = new ClientDataContainer{ClientData, CompletionDelegate}; // Don't forget to delete
	static auto proxy = ScreamAPI::proxyFunction<decltype(EOS_Ecom_QueryOwnership)>(__func__);
	proxy(Handle, Options, container, ScreamAPIcompletionDelegate);
}