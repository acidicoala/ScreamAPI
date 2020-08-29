#include "pch.h"
#include "eos-sdk\eos_achievements.h"
#include "ScreamAPI.h"
#include "util.h"

using namespace Util;

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryDefinitions(EOS_HAchievements Handle, const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	// Print out "Hidden Achievements"
	for(unsigned int i = 0; i < Options->HiddenAchievementsCount; i++){
		Logger::ach("\t""Hidden Achievement ID: %s", Options->HiddenAchievementIds[0]);
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_QueryDefinitions, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetAchievementDefinitionCount(EOS_HAchievements Handle, const EOS_Achievements_GetAchievementDefinitionCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_GetAchievementDefinitionCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::ach("Achievement Definition Count: %d", result);

	return result;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyAchievementDefinitionV2ByIndex, __func__);
	auto result = proxy(Handle, Options, OutDefinition);

	printAchievementDefinition(*OutDefinition);

	return result;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId, __func__);
	auto result = proxy(Handle, Options, OutDefinition);

	printAchievementDefinition(*OutDefinition);

	return result;
}

void EOS_CALL QueryPlayerAchievementsDelegate(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data){
	Logger::debug(__func__);
	auto container = reinterpret_cast<QueryPlayerAchievementsContainer*>(Data->ClientData);

	// Here we are redirecting the program flow to query all achievement defintions
	// The original callack function will be called in the QueryDefinitions delegate

	static bool firstTimeQuery = true; // Query defintions only once
	if(Data->ResultCode == EOS_EResult::EOS_Success && firstTimeQuery){ // Only query definitions in case of success
		EOS_Achievements_QueryDefinitionsOptions QueryOptions = {
			EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST,
			ScreamAPI::ProductUserId,
			ScreamAPI::EpicAccountId,
			nullptr,
			0
		};
		EOS_Achievements_QueryDefinitions(ScreamAPI::hAchievements, &QueryOptions, container, queryDefinitionsComplete);
		firstTimeQuery = false;
	} else{
		auto modifiedData = const_cast<EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo*>(Data);
		modifiedData->ClientData = container->originalClientData;
		container->originalCompletionDelegate(Data);
		delete container;
	}
}

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryPlayerAchievements(EOS_HAchievements Handle, const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	auto container = new QueryPlayerAchievementsContainer{ClientData, CompletionDelegate};
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_QueryPlayerAchievements, __func__);
	proxy(Handle, Options, container, QueryPlayerAchievementsDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetPlayerAchievementCount(EOS_HAchievements Handle, const EOS_Achievements_GetPlayerAchievementCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_GetPlayerAchievementCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::ach("Player Achievement Count: %d", result);

	return result;
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByIndexOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyPlayerAchievementByIndex, __func__);
	auto result = proxy(Handle, Options, OutAchievement);

	auto playerAchievement = *OutAchievement;
	printPlayerAchievement(playerAchievement);

	// Update our achievement array if this achievement is unlocked
	if(playerAchievement->UnlockTime != -1){
		// Find the corresponding achievement in our achievement array
		for(auto& achievement : ScreamAPI::achievements){
			// Compare by AchievementId
			if(!strcmp(achievement.AchievementId, playerAchievement->AchievementId)){
				// Found it
				achievement.UnlockState = UnlockState::Unlocked;
				break;
			}
		}
	}

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

// This is where the achievement magic happens ;)
EOS_DECLARE_FUNC(void) EOS_Achievements_UnlockAchievements(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	Logger::ach("Game requested to unlock %d achievement(s)", Options->AchievementsCount);
	for(unsigned int i = 0; i < Options->AchievementsCount; i++){
		Logger::ach("\t""Achievement ID: %s", Options->AchievementIds[i]);
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_UnlockAchievements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

// A proxy implementation so that we could call it ourselves
EOS_DECLARE_FUNC(void) EOS_Achievements_DefinitionV2_Release(EOS_Achievements_DefinitionV2* AchievementDefinition){
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_DefinitionV2_Release, __func__);
	proxy(AchievementDefinition);
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