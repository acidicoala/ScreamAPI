// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

/*
 * This file contains the deprecated types for EOS Achievements. In a future version, these types will be removed.
 */

#pragma pack(push, 8)

 /** The most recent version of the EOS_Achievements_Definition struct. */
#define EOS_ACHIEVEMENTS_DEFINITION_API_LATEST 1

/**
 * Contains information about a single achievement definition with localized text.
 */
EOS_STRUCT(EOS_Achievements_Definition, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Achievement ID that can be used to uniquely identify the achievement. */
	const char* AchievementId;
	/** Text representing the Name to display in-game when achievement has been unlocked. */
	const char* DisplayName;
	/** Text representing the description to display in-game when achievement has been unlocked. */
	const char* Description;
	/** Text representing the name to display in-game when achievement is locked. */
	const char* LockedDisplayName;
	/** Text representing the description of what needs to be done to trigger the unlock of this achievement. */
	const char* LockedDescription;
	/** Text representing the description to display in-game when achievement is hidden. */
	const char* HiddenDescription;
	/** Text representing the description of what happens when the achievement is unlocked. */
	const char* CompletionDescription;
	/** Text representing the icon to display in-game when achievement is unlocked. */
	const char* UnlockedIconId;
	/** Text representing the icon to display in-game when achievement is locked. */
	const char* LockedIconId;
	/** True if achievement is hidden, false otherwise. */
	EOS_Bool bIsHidden;
	/** The number of stat thresholds. */
	int32_t StatThresholdsCount;
	/** Array of stat thresholds that need to be satisfied to unlock the achievement. */
	const EOS_Achievements_StatThresholds* StatThresholds;
));

/**
 * Release the memory associated with achievement definitions. This must be called on data retrieved from
 * EOS_Achievements_CopyAchievementDefinitionByIndex or EOS_Achievements_CopyAchievementDefinitionByAchievementId.
 *
 * @param AchievementDefinition - The achievement definition to release.
 *
 * @see EOS_Achievements_Definition
 * @see EOS_Achievements_CopyAchievementDefinitionByIndex
 * @see EOS_Achievements_CopyAchievementDefinitionByAchievementId
 */
EOS_DECLARE_FUNC(void) EOS_Achievements_Definition_Release(EOS_Achievements_Definition* AchievementDefinition);

/** The most recent version of the EOS_Achievements_CopyAchievementDefinitionByIndexOptions struct. */
#define EOS_ACHIEVEMENTS_COPYDEFINITIONBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_CopyAchievementDefinitionByIndex Function.
 */
EOS_STRUCT(EOS_Achievements_CopyAchievementDefinitionByIndexOptions, (
	/** API Version of the EOS_Achievements_CopyAchievementDefinitionByIndexOptions function */
	int32_t ApiVersion;
	/** Index of the achievement definition to retrieve from the cache */
	uint32_t AchievementIndex;
));

/** The most recent version of the EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions struct. */
#define EOS_ACHIEVEMENTS_COPYDEFINITIONBYACHIEVEMENTID_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_CopyAchievementDefinitionByAchievementId Function.
 */
EOS_STRUCT(EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions, (
	/** API Version of the EOS_Achievements_CopyAchievementDefinitionByAchievementIdOptions function */
	int32_t ApiVersion;
	/** Achievement ID to look for when copying definition from the cache */
	const char* AchievementId;
));

/** The most recent version of the EOS_Achievements_UnlockedAchievement struct. */
#define EOS_ACHIEVEMENTS_UNLOCKEDACHIEVEMENT_API_LATEST 1

/**
 * Contains information about a single unlocked achievement.
 */
EOS_STRUCT(EOS_Achievements_UnlockedAchievement, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Achievement ID that can be used to uniquely identify the unlocked achievement. */
	const char* AchievementId;
	/** If not EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED then this is the POSIX timestamp that the achievement was unlocked. */
	int64_t UnlockTime;
));

/** The most recent version of the EOS_Achievements_GetUnlockedAchievementCount API. */
#define EOS_ACHIEVEMENTS_GETUNLOCKEDACHIEVEMENTCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_GetUnlockedAchievementCount Function.
 */
EOS_STRUCT(EOS_Achievements_GetUnlockedAchievementCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID for user for which to retrieve the unlocked achievement count */
	EOS_ProductUserId UserId;
));

/** The most recent version of the EOS_Achievements_CopyUnlockedAchievementByIndexOptions struct. */
#define EOS_ACHIEVEMENTS_COPYUNLOCKEDACHIEVEMENTBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_CopyUnlockedAchievementByIndex Function.
 */
EOS_STRUCT(EOS_Achievements_CopyUnlockedAchievementByIndexOptions, (
	/** API Version of the EOS_Achievements_CopyUnlockedAchievementByIndexOptions function */
	int32_t ApiVersion;
	/** The Account ID for the user who is copying the unlocked achievement. */
	EOS_ProductUserId UserId;
	/** Index of the unlocked achievement to retrieve from the cache */
	uint32_t AchievementIndex;
));

/** The most recent version of the EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions struct. */
#define EOS_ACHIEVEMENTS_COPYUNLOCKEDACHIEVEMENTBYACHIEVEMENTID_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_CopyUnlockedAchievementByAchievementId Function.
 */
EOS_STRUCT(EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions, (
	/** API Version of the EOS_Achievements_CopyUnlockedAchievementByAchievementIdOptions function */
	int32_t ApiVersion;
	/** The Account ID for the user who is copying the unlocked achievement. */
	EOS_ProductUserId UserId;
	/** AchievementId of the unlocked achievement to retrieve from the cache */
	const char* AchievementId;
));


/** The most recent version of the EOS_Achievements_AddNotifyAchievementsUnlocked API. */
#define EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKED_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_AddNotifyAchievementsUnlocked Function.
 */
EOS_STRUCT(EOS_Achievements_AddNotifyAchievementsUnlockedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Achievements_OnAchievementsUnlockedCallback Function.
 */
EOS_STRUCT(EOS_Achievements_OnAchievementsUnlockedCallbackInfo, (
	/** Context that was passed into EOS_Achievements_AddNotifyAchievementsUnlocked */
	void* ClientData;
	/** Account ID for user that received the unlocked achievements notification */
	EOS_ProductUserId UserId;
	/** The number of achievements. */
	uint32_t AchievementsCount;
	/** This member is not used and will always be set to NULL. */
	const char** AchievementIds;
));

/**
 * Function prototype definition for notifications that come from EOS_Achievements_AddNotifyAchievementsUnlocked
 *
 * @param Data A EOS_Achievements_OnAchievementsUnlockedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Achievements_OnAchievementsUnlockedCallback, const EOS_Achievements_OnAchievementsUnlockedCallbackInfo* Data);

/**
 * Release the memory associated with an unlocked achievement. This must be called on data retrieved from
 * EOS_Achievements_CopyUnlockedAchievementByIndex or EOS_Achievements_CopyUnlockedAchievementByAchievementId.
 *
 * @param Achievement - The unlocked achievement data to release.
 *
 * @see EOS_Achievements_UnlockedAchievement
 * @see EOS_Achievements_CopyUnlockedAchievementByIndex
 * @see EOS_Achievements_CopyUnlockedAchievementByAchievementId
 */
EOS_DECLARE_FUNC(void) EOS_Achievements_UnlockedAchievement_Release(EOS_Achievements_UnlockedAchievement* Achievement);

#pragma pack(pop)
