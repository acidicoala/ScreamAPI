// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_UserInfoHandle* EOS_HUserInfo;

/**
 * EOS_UserInfo_QueryUserInfo is used to start an asynchronous query to retrieve information, such as display name, about another account.
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_UserInfo_QueryUserInfo API. */
#define EOS_USERINFO_QUERYUSERINFO_API_LATEST 1

/**
 * Input parameters for the EOS_UserInfo_QueryUserInfo Function.
 */
EOS_STRUCT(EOS_UserInfo_QueryUserInfoOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Output parameters for the EOS_UserInfo_QueryUserInfo Function.
 */
EOS_STRUCT(EOS_UserInfo_QueryUserInfoCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_UserInfo_QueryUserInfo */
	void* ClientData;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_UserInfo_QueryUserInfo
 * @param Data A EOS_UserInfo_QueryUserInfoCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_UserInfo_OnQueryUserInfoCallback, const EOS_UserInfo_QueryUserInfoCallbackInfo* Data);


/**
 * EOS_UserInfo_QueryUserInfoByDisplayName is used to start an asynchronous query to retrieve user information by display name. This can be useful for getting the EOS_EpicAccountId for a display name.
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_UserInfo_QueryUserInfoByDisplayName API. */
#define EOS_USERINFO_QUERYUSERINFOBYDISPLAYNAME_API_LATEST 1

/**
 * Input parameters for the EOS_UserInfo_QueryUserInfoByDisplayName Function.
 */
EOS_STRUCT(EOS_UserInfo_QueryUserInfoByDisplayNameOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Display name of the player being queried */
	const char* DisplayName;
));

/**
 * Output parameters for the EOS_UserInfo_QueryUserInfoByDisplayName Function.
 */
EOS_STRUCT(EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_UserInfo_QueryUserInfoByDisplayName */
	void* ClientData;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
	/** Display name of the player being queried. This memory is only valid during the scope of the callback. */
	const char* DisplayName;
));

/**
 * Function prototype definition for callbacks passed to EOS_UserInfo_QueryUserInfo
 * @param Data A EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback, const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data);


/**
 * EOS_UserInfo_CopyUserInfo is used to immediately retrieve user information for an account id, cached by a previous call to EOS_UserInfo_QueryUserInfo.
 * The following types are used to work with the API.
 */

/** The maximum length of display names, in displayable characters */
#define EOS_USERINFO_MAX_DISPLAYNAME_CHARACTERS 16

/** The maximum length of display names when encoded as UTF-8 as returned by EOS_UserInfo_CopyUserInfo. This length does not include the null terminator. */
#define EOS_USERINFO_MAX_DISPLAYNAME_UTF8_LENGTH 64

 /** The most recent version of the EOS_UserInfo_CopyUserInfo API. */
#define EOS_USERINFO_COPYUSERINFO_API_LATEST 2

/** A structure that contains the user information. These structures are created by EOS_UserInfo_CopyUserInfo and must be passed to EOS_UserInfo_Release. */
EOS_STRUCT(EOS_UserInfo, (
	/** Version of the structure. This value is matched to the API version of EOS_UserInfo_CopyUserInfo. */
	int32_t ApiVersion;
	/** The account id of the user */
	EOS_EpicAccountId UserId;
	/** The name of the owner's country. This may be null */
	const char* Country;
	/** The display name. This may be null */
	const char* DisplayName;
	/** The ISO 639 language code for the user's preferred language. This may be null */
	const char* PreferredLanguage;
	/** A nickname/alias for the target user assigned by the local user. This may be null */
	const char* Nickname;
));

/**
 * Input parameters for the EOS_UserInfo_CopyUserInfo Function.
 */
EOS_STRUCT(EOS_UserInfo_CopyUserInfoOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Release the memory associated with an EOS_UserInfo structure. This must be called on data retrieved from EOS_UserInfo_CopyUserInfo.
 *
 * @param UserInfo - The user info structure to release
 *
 * @see EOS_UserInfo
 * @see EOS_UserInfo_CopyUserInfo
 */
EOS_DECLARE_FUNC(void) EOS_UserInfo_Release(EOS_UserInfo* UserInfo);

/** The most recent version of the EOS_UserInfo_External struct. */
#define EOS_USERINFO_EXTERNALUSERINFO_API_LATEST 1

/**
 * Contains information about a single external user info.
 */
EOS_STRUCT(EOS_UserInfo_ExternalUserInfo, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** The type of the external account */
	EOS_EExternalAccountType AccountType;
	/** The id of the external account. Can be null */
	const char* AccountId;
	/** The display name of the external account. Can be null */
	const char* DisplayName;
));

/** The most recent version of the EOS_Achievements_GetAchievementDefinitionCount API. */
#define EOS_USERINFO_GETEXTERNALUSERINFOCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_UserInfo_GetExternalUserInfoCount Function.
 */
EOS_STRUCT(EOS_UserInfo_GetExternalUserInfoCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
));

/** The most recent version of the EOS_UserInfo_CopyExternalUserInfoByIndexOptions struct. */
#define EOS_USERINFO_COPYEXTERNALUSERINFOBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_UserInfo_CopyExternalUserInfoByIndex Function.
 */
EOS_STRUCT(EOS_UserInfo_CopyExternalUserInfoByIndexOptions, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
	/** Index of the external user info to retrieve from the cache */
	uint32_t Index;
));

/** The most recent version of the EOS_UserInfo_CopyExternalUserInfoByIndexOptions struct. */
#define EOS_USERINFO_COPYEXTERNALUSERINFOBYACCOUNTTYPE_API_LATEST 1

/**
 * Input parameters for the EOS_UserInfo_CopyExternalUserInfoByAccountType function.
 */
EOS_STRUCT(EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
	/** Account type of the external user info to retrieve from the cache */
	EOS_EExternalAccountType AccountType;
));

/** The most recent version of the EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions struct. */
#define EOS_USERINFO_COPYEXTERNALUSERINFOBYACCOUNTID_API_LATEST 1

/**
 * Input parameters for the EOS_UserInfo_CopyExternalUserInfoByAccountId Function.
 */
EOS_STRUCT(EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions, (
	/** Version of the API. */
	int32_t ApiVersion;
	/** Account ID of the local player requesting the information */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the player whose information is being retrieved */
	EOS_EpicAccountId TargetUserId;
	/** Account ID of the external user info to retrieve from the cache. Cannot be null */
	const char* AccountId;
));

/**
 * Release the memory associated with external user info. This must be called on data retrieved from
 * EOS_UserInfo_CopyExternalUserInfoByIndexOptions.
 *
 * @param ExternalUserInfo The external user info to release.
 *
 * @see EOS_UserInfo_ExternalUserInfo
 * @see EOS_UserInfo_CopyExternalUserInfoByIndex
 */
EOS_DECLARE_FUNC(void) EOS_UserInfo_ExternalUserInfo_Release(EOS_UserInfo_ExternalUserInfo* ExternalUserInfo);

#pragma pack(pop)
