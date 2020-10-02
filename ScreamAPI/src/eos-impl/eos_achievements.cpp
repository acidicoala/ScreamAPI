#include "pch.h"
#include "eos-sdk/eos_achievements.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"

/* Achievement Definitions */

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryDefinitions(EOS_HAchievements Handle, const EOS_Achievements_QueryDefinitionsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryDefinitionsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);
	
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
	Logger::debug(__func__);
	
	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyAchievementDefinitionV2ByIndex, __func__);
	return proxy(Handle, Options, OutDefinition);
}

EOS_DECLARE_FUNC(void) EOS_Achievements_DefinitionV2_Release(EOS_Achievements_DefinitionV2* AchievementDefinition){
	//Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_DefinitionV2_Release, __func__);
	proxy(AchievementDefinition);
}

/* Player Achievements */

EOS_DECLARE_FUNC(void) EOS_Achievements_QueryPlayerAchievements(EOS_HAchievements Handle, const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_QueryPlayerAchievements, __func__);
	proxy(Handle, Options, ClientData, CompletionDelegate);
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
	return proxy(Handle, Options, OutAchievement);
}

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyPlayerAchievementByAchievementId(EOS_HAchievements Handle, const EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions* Options, EOS_Achievements_PlayerAchievement** OutAchievement){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyPlayerAchievementByAchievementId, __func__);
	return proxy(Handle, Options, OutAchievement);
}

EOS_DECLARE_FUNC(void) EOS_Achievements_PlayerAchievement_Release(EOS_Achievements_PlayerAchievement* Achievement){
	//Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_PlayerAchievement_Release, __func__);
	proxy(Achievement);
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

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlockedV2(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedV2Options* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallbackV2 NotificationFn){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_AddNotifyAchievementsUnlockedV2, __func__);
	return proxy(Handle, Options, ClientData, NotificationFn);
}

/* Deprecated Functions */

EOS_DECLARE_FUNC(EOS_EResult) EOS_Achievements_CopyAchievementDefinitionByIndex(EOS_HAchievements Handle, const EOS_Achievements_CopyAchievementDefinitionByIndexOptions* Options, EOS_Achievements_Definition** OutDefinition){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_CopyAchievementDefinitionByIndex, __func__);
	return proxy(Handle, Options, OutDefinition);
}

EOS_DECLARE_FUNC(void) EOS_Achievements_Definition_Release(EOS_Achievements_Definition* AchievementDefinition){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_Definition_Release, __func__);
	proxy(AchievementDefinition);
}

EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Achievements_AddNotifyAchievementsUnlocked(EOS_HAchievements Handle, const EOS_Achievements_AddNotifyAchievementsUnlockedOptions* Options, void* ClientData, const EOS_Achievements_OnAchievementsUnlockedCallback NotificationFn){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Achievements_AddNotifyAchievementsUnlocked, __func__);
	return proxy(Handle, Options, ClientData, NotificationFn);
}