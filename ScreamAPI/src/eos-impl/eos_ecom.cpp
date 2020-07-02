#include "pch.h"
#include <ScreamAPI.h>



EOS_DECLARE_FUNC(void) EOS_Ecom_QueryOwnership(EOS_HEcom Handle, const EOS_Ecom_QueryOwnershipOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOwnershipCallback CompletionDelegate){
	//static auto proxy = ScreamAPI::proxyFunction<decltype(EOS_Ecom_QueryOwnership)>(__FUNCTION__); // TODO: DELETE later
	//proxy(Handle, Options, ClientData, CompletionDelegate);
	// Log item IDs
	
	if(Config::isLoggingDLCQueries()){
		if(Options){
			Logger::info("Game requested ownership of %d items:", Options->CatalogItemIdCount);

			for(uint32_t i = 0; i < Options->CatalogItemIdCount; i++)
				Logger::info(" - Item ID: %s", Options->CatalogItemIds[i]);
		} else{
			Logger::warn("Game requested ownership, but without Options parameter");
		}
	}

	if(Options)	// Verify SDK version
		ScreamAPI::checkSdkVersion(Options->ApiVersion, EOS_ECOM_QUERYOWNERSHIP_API_LATEST);

	// Generate fake result

	// Don't forget to clear up once done
	EOS_Ecom_QueryOwnershipCallbackInfo* cbi = new EOS_Ecom_QueryOwnershipCallbackInfo;
	EOS_Ecom_ItemOwnership* ownerships = nullptr;
	std::vector<std::string> catalogIDs;

	if(Options){
		// Don't forget to clear up once done
		ownerships = new EOS_Ecom_ItemOwnership[Options->CatalogItemIdCount];

		for(uint32_t i = 0; i < Options->CatalogItemIdCount; i++){
			// Save the id, so we can restore it in the callback
			catalogIDs.push_back(std::string(Options->CatalogItemIds[i]));
			bool isInOwnedList = std::find(Config::getOwnedItemIDs().begin(),
										   Config::getOwnedItemIDs().end(),
										   Options->CatalogItemIds[i]) != Config::getOwnedItemIDs().end();

			bool owned = Config::ownAllDLC() || isInOwnedList;
			auto ownership = owned ? EOS_EOwnershipStatus::EOS_OS_Owned : EOS_EOwnershipStatus::EOS_OS_NotOwned;

			ownerships[i].ApiVersion = Options->ApiVersion;
			ownerships[i].Id = nullptr; // Game will relocate the string anyway
			ownerships[i].OwnershipStatus = ownership;
		}
		cbi->ItemOwnershipCount = Options->CatalogItemIdCount;
		cbi->ItemOwnership = nullptr; // It will relocate the array anyway
		cbi->LocalUserId = Options->LocalUserId;
		cbi->ResultCode = EOS_EResult::EOS_Success;
	} else{
		// Really should not be the case that Options are null
		cbi->ItemOwnershipCount = 0;
		cbi->ItemOwnership = nullptr;
		cbi->LocalUserId = nullptr;
		cbi->ResultCode = EOS_EResult::EOS_InvalidParameters;
	}
	cbi->ClientData = ClientData;

	ScreamAPI::addCallback([=] (){
		if(ownerships){
			if(cbi->ItemOwnershipCount > 0)
				Logger::info("Responding with %d item ownerships:", cbi->ItemOwnershipCount);

			// Restore itemIDs to ownerships
			for(uint32_t i = 0; i < cbi->ItemOwnershipCount; i++){
				ownerships[i].Id = catalogIDs.at(i).c_str();

				if(Config::isLoggingDLCQueries()){
					auto status_str = ownerships[i].OwnershipStatus == EOS_EOwnershipStatus::EOS_OS_Owned ? "Owned" : "NotOwned";
					Logger::info(" - Item ID: %s\t[%s]", ownerships[i].Id, status_str);
				}
			}
		}
		cbi->ItemOwnership = ownerships;
		CompletionDelegate(cbi);

		delete[] cbi->ItemOwnership;
		delete cbi;
	});
}