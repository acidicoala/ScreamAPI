#include "pch.h"
#include "eos-sdk/eos_achievements.h"
#include <ScreamAPI.h>
#include <Overlay.h>


EOS_DECLARE_FUNC(void) EOS_Achievements_QueryDefinitions(EOS_HAchievements Handle, const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	// Print out "Hidden Achievements"
	for(unsigned int i = 0; i < Options->HiddenAchievementsCount; i++){
		Logger::debug("\t""Hidden Achievement ID: %s", Options->HiddenAchievementIds[0]);
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_QueryDefinitions, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetAchievementDefinitionCount(EOS_HAchievements Handle, const EOS_Achievements_GetAchievementDefinitionCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_GetAchievementDefinitionCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::debug("\t""Achievement Definition Count: %d", result);

	return result;
}

// Place this function in some util file?
void printAchievementDefinition(EOS_Achievements_DefinitionV2* definition){
	if(definition == nullptr){
		Logger::error("\t""Invalid Achievement Definition");
	} else{
		Logger::debug(" - Achievement Definition");
		Logger::debug("\t\t""AchievementId: %s", definition->AchievementId);
		Logger::debug("\t\t""IsHidden: %d", definition->bIsHidden);
		Logger::debug("\t\t""FlavorText: %s", definition->FlavorText);
		Logger::debug("\t\t""LockedDescription: %s", definition->LockedDescription);
		Logger::debug("\t\t""LockedDisplayName: %s", definition->LockedDisplayName);
		Logger::debug("\t\t""LockedIconURL: %s", definition->LockedIconURL);
		for(unsigned int i = 0; i < definition->StatThresholdsCount; i++){
			Logger::debug("\t\t\t""[StatThreshold] Name: %s; Threshold: %d",
						  definition->StatThresholds[i].Name,
						  definition->StatThresholds[i].Threshold);
		}
		Logger::debug("\t\t""StatThresholdsCount: %d", definition->StatThresholdsCount);
		Logger::debug("\t\t""UnlockedDescription: %s", definition->UnlockedDescription);
		Logger::debug("\t\t""UnlockedDisplayName: %s", definition->UnlockedDisplayName);
		Logger::debug("\t\t""UnlockedIconURL: %s", definition->UnlockedIconURL);
	}
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyAchievementDefinitionV2ByIndex, __func__);
	auto result = proxy(Handle, Options, OutDefinition);

	printAchievementDefinition(*OutDefinition);

	return result;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId, __func__);
	auto result = proxy(Handle, Options, OutDefinition);

	printAchievementDefinition(*OutDefinition);

	return result;
}

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryPlayerAchievements(EOS_HAchievements Handle, const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_QueryPlayerAchievements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetPlayerAchievementCount(EOS_HAchievements Handle, const EOS_Achievements_GetPlayerAchievementCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_GetPlayerAchievementCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::debug(" - Player Achievement Count: %d", result);
	
	// TODO: Move this init to a better place?
	Overlay::init();


	return result;
}


// Place this function in some util file?
void printPlayerAchievement(EOS_Achievements_PlayerAchievement* achievement){
	if(achievement == nullptr){
		Logger::error(" - Invalid Player Achievement");
	} else{
		Logger::debug(" - Player Achievement");
		Logger::debug("\t\t""AchievementId: %s", achievement->AchievementId);
		Logger::debug("\t\t""ApiVersion: %d", achievement->ApiVersion);
		Logger::debug("\t\t""Description: %s", achievement->Description);
		Logger::debug("\t\t""DisplayName: %s", achievement->DisplayName);
		Logger::debug("\t\t""FlavorText: %s", achievement->FlavorText);
		Logger::debug("\t\t""IconURL: %s", achievement->IconURL);
		Logger::debug("\t\t""Progress: %f", achievement->Progress);
		for(int i = 0; i < achievement->StatInfoCount; i++){
			Logger::debug("\t\t\t""[StatInfo] Name: %s, CurrentValue: %d, ThresholdValue: %d",
						  achievement->StatInfo[i].Name,
						  achievement->StatInfo[i].CurrentValue,
						  achievement->StatInfo[i].ThresholdValue);
		}
		Logger::debug("\t\t""StatInfoCount: %d", achievement->StatInfoCount);
		Logger::debug("\t\t""UnlockTime: %lld", achievement->UnlockTime);
	}
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByIndexOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyPlayerAchievementByIndex, __func__);
	auto result = proxy(Handle, Options, OutAchievement);

	printPlayerAchievement(*OutAchievement);

	return result;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyPlayerAchievementByAchievementId, __func__);
	auto result = proxy(Handle, Options, OutAchievement);

	printPlayerAchievement(*OutAchievement);

	return result;
}

struct UnlockAchievementsContainer{
	void* originalClientData;
	EOS_Achievements_OnUnlockAchievementsCompleteCallback originalCompletionDelegate;
};

void EOS_CALL UnlockAchievementsDelegate(const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data){
	Logger::debug(__func__);

	auto container = reinterpret_cast<UnlockAchievementsContainer*>(Data->ClientData);

	// get non-const pointer to data
	auto modifiedData = const_cast<EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo*>(Data);

	// Restore original client data
	modifiedData->ClientData = container->originalClientData;

	// Call original completion delegate with our modified data
	container->originalCompletionDelegate(Data);

	// Free the heap
	delete container;
}

// This is where the achievement magic happens ;)
EOS_DECLARE_FUNC(void) EOS_Achievements_UnlockAchievements(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	Logger::debug("Game requested to unlock %d achievement(s)");
	for(unsigned int i = 0; i < Options->AchievementsCount; i++){
		Logger::debug("\t""Achievement ID: %s", Options->AchievementIds[i]);
	}

	auto container = new UnlockAchievementsContainer{ClientData, CompletionDelegate}; // Don't forget to free the heap
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_UnlockAchievements, __func__);
	proxy(Handle, Options, container, UnlockAchievementsDelegate);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlockedV2(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedV2Options* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallbackV2 NotificationFn){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_AddNotifyAchievementsUnlockedV2, __func__);
	auto result = proxy(Handle, Options, ClientData, NotificationFn);

	return result;
}

EOS_DECLARE_FUNC(void) EOS_Achievements_RemoveNotifyAchievementsUnlocked(EOS_HAchievements Handle, EOS_NotificationId InId){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_RemoveNotifyAchievementsUnlocked, __func__);
	proxy(Handle, InId);
}

// Deprecated
/*
EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionByIndexOptions* Options, EOS_Achievements_Definition** OutDefinition);
EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions* Options, EOS_Achievements_Definition** OutDefinition);
EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetUnlockedAchievementCount(EOS_HAchievements Handle, const EOS_Achievements_GetUnlockedAchievementCountOptions* Options);
EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyUnlockedAchievementByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyUnlockedAchievementByIndexOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement);
EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyUnlockedAchievementByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions* Options, EOS_Achievements_UnlockedAchievement** OutAchievement);
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlocked(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedOptions* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallback NotificationFn);
*/