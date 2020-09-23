#include "pch.h"
#include "ScreamAPI.h"
#include "eos-sdk/eos_ecom.h"
#include "util.h"

EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnership(EOS_HEcom Handle, const EOS_Ecom_QueryOwnershipOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate){
	Logger::debug(__func__);

	// Log item IDs
	if(Options){
		Logger::dlc("Game requested ownership of %d items:", Options->CatalogItemIdCount);
		for(uint32_t i = 0; i < Options->CatalogItemIdCount; i++)
			Logger::dlc("\t""Item ID: %s", Options->CatalogItemIds[i]);
	}
	else{
		Logger::warn("Game requested DLC ownership, but without Options parameter");
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Ecom_QueryOwnership, __func__);

	if(Config::EnableItemUnlocker()){
		auto container = new ScreamAPI::OriginalDataContainer(ClientData, CompletionDelegate);
		proxy(Handle, Options, container, [](const EOS_Ecom_QueryOwnershipCallbackInfo* Data){
			auto nonConstData = const_cast<EOS_Ecom_QueryOwnershipCallbackInfo*>(Data);
			ScreamAPI::proxyCallback(nonConstData, &nonConstData->ClientData, [&](){
				Logger::dlc("Responding with %d item ownerships:", nonConstData->ItemOwnershipCount);

				// Modify ownership status of items.
				for(unsigned int i = 0; i < nonConstData->ItemOwnershipCount; i++){
					// Get non-const pointer to ownership struct
					auto item = const_cast <EOS_Ecom_ItemOwnership*>(nonConstData->ItemOwnership + i);

					// Search the id in DLC list from the config
					bool isInOwnedList = Util::vectorContains<std::string>(Config::DLC_List(), nonConstData->ItemOwnership[i].Id);

					// Determine if this DLC should be unlocked
					bool unlocked = Config::UnlockAllDLC() || isInOwnedList;

					// Finally, change the ownership status
					item->OwnershipStatus = unlocked ? EOS_EOwnershipStatus::EOS_OS_Owned : EOS_EOwnershipStatus::EOS_OS_NotOwned;

					auto ownershipStatusString = item->OwnershipStatus == EOS_EOwnershipStatus::EOS_OS_Owned ? "Owned" : "Not Owned";
					Logger::dlc("\t""Item ID: %s\t[%s]", item->Id, ownershipStatusString);
				}
			});
		});
	}
	else{
		proxy(Handle, Options, ClientData, CompletionDelegate);
	}
}
