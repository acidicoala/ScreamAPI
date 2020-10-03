#include "pch.h"
#include "eos-sdk/eos_achievements.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"


EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetPlayerAchievementCount(EOS_HAchievements Handle, const EOS_Achievements_GetPlayerAchievementCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_GetPlayerAchievementCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::ach("Player Achievement Count: %d", result);

	return result;
}

// This is where the achievement magic happens ;)
EOS_DECLARE_FUNC(void) EOS_Achievements_UnlockAchievements(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	Logger::info("Game requested to unlock %d achievement(s)", Options->AchievementsCount);
	for(unsigned int i = 0; i < Options->AchievementsCount; i++){
		Logger::info("\t""Achievement ID: %s", Options->AchievementIds[i]);
	}

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_UnlockAchievements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(uint32_t) EOS_Achievements_GetAchievementDefinitionCount(EOS_HAchievements Handle, const EOS_Achievements_GetAchievementDefinitionCountOptions* Options){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_GetAchievementDefinitionCount, __func__);
	auto result = proxy(Handle, Options);

	Logger::ach("Achievement Definition Count: %d", result);

	return result;
}

/* Standard Proxy Implementations */

// Achievement Definitions

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryDefinitions(EOS_HAchievements Handle, const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_QueryDefinitions, Handle, Options, ClientData, CompletionDelegate);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionV2ByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* Options, EOS_Achievements_DefinitionV2** OutDefinition){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_CopyAchievementDefinitionV2ByIndex, Handle, Options, OutDefinition);
}

EOS_DECLARE_FUNC(void) EOS_Achievements_DefinitionV2_Release(EOS_Achievements_DefinitionV2* AchievementDefinition){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_DefinitionV2_Release, AchievementDefinition)
}

// Player Achievements

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryPlayerAchievements(EOS_HAchievements Handle, const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_QueryPlayerAchievements, Handle, Options, ClientData, CompletionDelegate)
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByIndexOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_CopyPlayerAchievementByIndex, Handle, Options, OutAchievement);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_CopyPlayerAchievementByAchievementId, Handle, Options, OutAchievement);
}

EOS_DECLARE_FUNC(void) EOS_Achievements_PlayerAchievement_Release(EOS_Achievements_PlayerAchievement* Achievement){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_PlayerAchievement_Release, Achievement);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlockedV2(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedV2Options* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallbackV2 NotificationFn){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_AddNotifyAchievementsUnlockedV2, Handle, Options, ClientData, NotificationFn);
}

// Deprecated Functions

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionByIndexOptions* Options, EOS_Achievements_Definition** OutDefinition){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_CopyAchievementDefinitionByIndex, Handle, Options, OutDefinition);
}

EOS_DECLARE_FUNC(void) EOS_Achievements_Definition_Release(EOS_Achievements_Definition* AchievementDefinition){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_Definition_Release, AchievementDefinition);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlocked(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedOptions* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallback NotificationFn){
	EOS_IMPLEMENT_FUNC(EOS_Achievements_AddNotifyAchievementsUnlocked, Handle, Options, ClientData, NotificationFn);
}