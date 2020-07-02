// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_AchievementsHandle* EOS_HAchievements;

/** The most recent version of the EOS_Achievements_QueryDefinitions struct. */
#define EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_QueryDefinitions Function.
 */
EOS_STRUCT(EOS_Achievements_QueryDefinitionsOptions, (
	/** API Version. */
	int32_t ApiVersion;
	/** Product User ID for user who is querying definitions, if not valid default text will be returned. */
	EOS_ProductUserId UserId;
	/** Epic account ID for user who is querying definitions, if not valid default text will be returned. */
	EOS_EpicAccountId EpicUserId;
	/** An array of Achievement IDs for hidden achievements to get full details for. */
	const char** HiddenAchievementIds;
	/** The number of hidden achievements to get full details for. */
	uint32_t HiddenAchievementsCount;
));

#define EOS_ACHIEVEMENTS_STATTHRESHOLD_API_LATEST 1
/**
 * Contains information about a collection of stat threshold data.
 *
 * The threshold will depend on the stat aggregate type:
 *   LATEST (Tracks the latest value)
 *   MAX (Tracks the maximum value)
 *   MIN (Tracks the minimum value)
 *   SUM (Generates a rolling sum of the value)
 *
 * @see EOS_Achievements_Definition
 */
EOS_STRUCT(EOS_Achievements_StatThresholds, (
	/** API Version for the EOS_Achievements_StatThresholds struct */
	int32_t ApiVersion;
	/** The name of the stat. */
	const char* Name;
	/** The value of this data. */
	int32_t Threshold;
));

#define EOS_ACHIEVEMENTS_PLAYERSTATINFO_API_LATEST 1
/**
 * Contains information about a collection of stat info data.
 *
 * @see EOS_Achievements_PlayerAchievement
 */
EOS_STRUCT(EOS_Achievements_PlayerStatInfo, (
	/** API Version for the EOS_Achievements_PlayerStatInfo struct */
	int32_t ApiVersion;
	/** The name of the stat. */
	const char* Name;
	/** The current value of the stat. */
	int32_t CurrentValue;
	/** The threshold value of the stat. */
	int32_t ThresholdValue;
));

/** The most recent version of the EOS_Achievements_DefinitionV2 struct. */
#define EOS_ACHIEVEMENTS_DEFINITIONV2_API_LATEST 2

/**
 * Contains information about a single achievement definition with localized text.
 */
EOS_STRUCT(EOS_Achievements_DefinitionV2, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Achievement ID that can be used to uniquely identify the achievement. */
	const char* AchievementId;
	/** Localized display name for the achievement when it has been unlocked. */
	const char* UnlockedDisplayName;
	/** Localized description for the achievement when it has been unlocked. */
	const char* UnlockedDescription;
	/** Localized display name for the achievement when it is locked or hidden. */
	const char* LockedDisplayName;
	/** Localized description for the achievement when it is locked or hidden. */
	const char* LockedDescription;
	/** Localized flavor text that can be used by the game in an arbitrary manner. This may be null if there is no data configured in the dev portal */
	const char* FlavorText;
	/** URL of an icon to display for the achievement when it is unlocked. This may be null if there is no data configured in the dev portal */
	const char* UnlockedIconURL;
	/** URL of an icon to display for the achievement when it is locked or hidden. This may be null if there is no data configured in the dev portal */
	const char* LockedIconURL;
	/** True if achievement is hidden, false otherwise. */
	EOS_Bool bIsHidden;
	/** The number of stat thresholds. */
	uint32_t StatThresholdsCount;
	/** Array of stat thresholds that need to be satisfied to unlock the achievement. */
	const EOS_Achievements_StatThresholds* StatThresholds;
));

/**
 * Release the memory associated with EOS_Achievements_DefinitionV2. This must be called on data retrieved from
 * EOS_Achievements_CopyAchievementDefinitionV2ByIndex or EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId.
 *
 * @param AchievementDefinition - The achievement definition to release.
 *
 * @see EOS_Achievements_DefinitionV2
 * @see EOS_Achievements_CopyAchievementDefinitionV2ByIndex
 * @see EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId
 */
EOS_DECLARE_FUNC(void) EOS_Achievements_DefinitionV2_Release(EOS_Achievements_DefinitionV2* AchievementDefinition);


/** The most recent version of the EOS_Achievements_GetAchievementDefinitionCount API. */
#define EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_GetAchievementDefinitionCount Function.
 */
EOS_STRUCT(EOS_Achievements_GetAchievementDefinitionCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_Achievements_CopyAchievementDefinitionByIndexOptions struct. */
#define EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYINDEX_API_LATEST 2

/**
 * Input parameters for the EOS_Achievements_CopyAchievementDefinitionByIndex Function.
 */
EOS_STRUCT(EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions, (
	/** API Version of the EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions function */
	int32_t ApiVersion;
	/** Index of the achievement definition to retrieve from the cache */
	uint32_t AchievementIndex;
));

/** The most recent version of the EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions struct. */
#define EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYACHIEVEMENTID_API_LATEST 2

/**
 * Input parameters for the EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId Function.
 */
EOS_STRUCT(EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions, (
	/** API Version of the EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions function */
	int32_t ApiVersion;
	/** Achievement ID to look for when copying definition from the cache */
	const char* AchievementId;
));

/**
 * Data containing the result information for a query definitions request.
 */
EOS_STRUCT(EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Achievements_QueryDefinitions. */
	void* ClientData;
));

/**
 * Function prototype definition for callbacks passed to EOS_Achievements_QueryDefinitions
 * @param Data A EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Achievements_OnQueryDefinitionsCompleteCallback, const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data);


/** The most recent version of the EOS_Achievements_QueryPlayerAchievements struct. */
#define EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_QueryPlayerAchievements Function.
 */
EOS_STRUCT(EOS_Achievements_QueryPlayerAchievementsOptions, (
	/** API Version. */
	int32_t ApiVersion;
	/** The account ID for the user whose achievements are to be retrieved. */
	EOS_ProductUserId UserId;
));

/** Timestamp value representing an undefined UnlockTime for EOS_Achievements_PlayerAchievement and EOS_Achievements_UnlockedAchievement */
#define EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED -1

/** The most recent version of the EOS_Achievements_PlayerAchievement struct. */
#define EOS_ACHIEVEMENTS_PLAYERACHIEVEMENT_API_LATEST 2

/**
 * Contains information about a single player achievement.
 */
EOS_STRUCT(EOS_Achievements_PlayerAchievement, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Achievement ID that can be used to uniquely identify the achievement. */
	const char* AchievementId;
	/** Progress towards completing this achievement (as a percentage). */
	double Progress;
	/** If not EOS_ACHIEVEMENTS_ACHIEVEMENT_UNLOCKTIME_UNDEFINED then this is the POSIX timestamp that the achievement was unlocked. */
	int64_t UnlockTime;
	/** The number of player stat info entries. */
	int32_t StatInfoCount;
	/** Array of player stat info. These values can be used to calculate the overall progress towards unlocking the achievement. */
	const EOS_Achievements_PlayerStatInfo* StatInfo;
	/** 
	 * Localized display name for the achievement based on this specific player's current progress on the achievement. 
	 * Note: The current progress is updated when EOS_Achievements_QueryPlayerAchievements succeeds and when an achievement is unlocked.
	 */
	const char* DisplayName;
	/** 
	 * Localized description for the achievement based on this specific player's current progress on the achievement.
	 * Note: The current progress is updated when EOS_Achievements_QueryPlayerAchievements succeeds and when an achievement is unlocked.
	 */
	const char* Description;
	/**
	 * URL of an icon to display for the achievement based on this specific player's current progress on the achievement. This may be null if there is no data configured in the dev portal
	 * Note: The current progress is updated when EOS_Achievements_QueryPlayerAchievements succeeds and when an achievement is unlocked.
	 */
	const char* IconURL;
	/** Localized flavor text that can be used by the game in an arbitrary manner. This may be null if there is no data configured in the dev portal */
	const char* FlavorText;
));

/** The most recent version of the EOS_Achievements_GetPlayerAchievementCount API. */
#define EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_GetPlayerAchievementCount Function.
 */
EOS_STRUCT(EOS_Achievements_GetPlayerAchievementCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID for the user for which to retrieve the achievement count */
	EOS_ProductUserId UserId;
));

/** The most recent version of the EOS_Achievements_CopyPlayerAchievementByIndexOptions struct. */
#define EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_CopyPlayerAchievementByIndex Function.
 */
EOS_STRUCT(EOS_Achievements_CopyPlayerAchievementByIndexOptions, (
	/** API Version of the EOS_Achievements_CopyPlayerAchievementByIndexOptions function */
	int32_t ApiVersion;
	/** The Account ID for the user who is copying the achievement. */
	EOS_ProductUserId UserId;
	/** Index of the achievement to retrieve from the cache */
	uint32_t AchievementIndex;
));

/** The most recent version of the EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions struct. */
#define EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYACHIEVEMENTID_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_CopyPlayerAchievementByAchievementId Function.
 */
EOS_STRUCT(EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions, (
	/** API Version of the EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions function */
	int32_t ApiVersion;
	/** The Account ID for the user who is copying the achievement. */
	EOS_ProductUserId UserId;
	/** Achievement ID to search for when retrieving player achievement data from the cache */
	const char* AchievementId;
));

/**
 * Release the memory associated with a player achievement. This must be called on data retrieved from
 * EOS_Achievements_CopyPlayerAchievementByIndex or EOS_Achievements_CopyPlayerAchievementByAchievementId.
 *
 * @param Achievement - The achievement data to release.
 *
 * @see EOS_Achievements_PlayerAchievement
 * @see EOS_Achievements_CopyPlayerAchievementByIndex
 * @see EOS_Achievements_CopyPlayerAchievementByAchievementId
 */
EOS_DECLARE_FUNC(void) EOS_Achievements_PlayerAchievement_Release(EOS_Achievements_PlayerAchievement* Achievement);

/**
 * Data containing the result information for querying a player's achievements request.
 */
EOS_STRUCT(EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Achievements_QueryPlayerAchievements. */
	void* ClientData;
	/** The Account ID of the user who initiated this request. */
	EOS_ProductUserId UserId;
));

 /**
  * Function prototype definition for callbacks passed to EOS_Achievements_QueryPlayerAchievements
  *
  * @param Data A EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo containing the output information and result
  *
  * @see EOS_Achievements_PlayerAchievement_Release
  */
EOS_DECLARE_CALLBACK(EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data);


/** The most recent version of the EOS_Achievements_UnlockAchievements struct. */
#define EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST 1

/**
 * Input parameters for the EOS_Achievements_UnlockAchievements Function.
 */
EOS_STRUCT(EOS_Achievements_UnlockAchievementsOptions, (
	/** API Version. */
	int32_t ApiVersion;
	/** The Account ID for the user whose achievements are to be unlocked. */
	EOS_ProductUserId UserId;
	/** An array of Achievement IDs to unlock. */
	const char** AchievementIds;
	/** The number of achievements to unlock. */
	uint32_t AchievementsCount;
));

/**
 * Data containing the result information for unlocking achievements request.
 */
EOS_STRUCT(EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful request, other codes indicate an error. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Achievements_UnlockAchievements. */
	void* ClientData;
	/** The Account ID of the user who initiated this request. */
	EOS_ProductUserId UserId;
	/** The number of achievements to unlock. */
	uint32_t AchievementsCount;
));

/**
 * Function prototype definition for callbacks passed to EOS_Achievements_UnlockAchievements
 * @param Data A EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Achievements_OnUnlockAchievementsCompleteCallback, const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data);

/** The most recent version of the EOS_Achievements_AddNotifyAchievementsUnlockedV2 API. */
#define EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKEDV2_API_LATEST 2

/**
 * Input parameters for the EOS_Achievements_AddNotifyAchievementsUnlocked Function.
 */
EOS_STRUCT(EOS_Achievements_AddNotifyAchievementsUnlockedV2Options, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Achievements_OnAchievementsUnlockedCallbackV2 Function.
 */
EOS_STRUCT(EOS_Achievements_OnAchievementsUnlockedCallbackV2Info, (
	/** Context that was passed into EOS_Achievements_AddNotifyAchievementsUnlocked */
	void* ClientData;
	/** Account ID for user that received the unlocked achievements notification */
	EOS_ProductUserId UserId;
	/** Achievement ID for the achievement that was unlocked. Can be passed to EOS_Achievements_CopyPlayerAchievementByAchievementId to get full information for the achievement. */
	const char* AchievementId;
	/** POSIX timestamp when the achievement was unlocked */
	int64_t UnlockTime;
));

/**
 * Function prototype definition for notifications that come from EOS_Achievements_AddNotifyAchievementsUnlockedV2
 *
 * @param Data A EOS_Achievements_OnAchievementsUnlockedCallbackV2Info containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Achievements_OnAchievementsUnlockedCallbackV2, const EOS_Achievements_OnAchievementsUnlockedCallbackV2Info* Data);


#pragma pack(pop)

#include "eos_achievements_types_deprecated.inl"
