// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_FriendsHandle* EOS_HFriends;

/**
 * EOS_Friends_QueryFriends is used to start an asynchronous query to retrieve friends and pending outbound/inbound friendship invitations for a user account.
 * The following types are used to work with the API.
 */


/** The most recent version of the EOS_Friends_QueryFriends API. */
#define EOS_FRIENDS_QUERYFRIENDS_API_LATEST 1

/**
 * Input parameters for the EOS_Friends_QueryFriends Function.
 */
EOS_STRUCT(EOS_Friends_QueryFriendsOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user whose friends should be asynchronously retrieved */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Output parameters for the EOS_Friends_QueryFriends Function. These parameters are received through the callback provided to EOS_Friends_QueryFriends
 */
EOS_STRUCT(EOS_Friends_QueryFriendsCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Friends_QueryFriends */
	void* ClientData;
	/** Account ID of the user whose friends were queried */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Friends_QueryFriends
 * @param Data A EOS_Friends_QueryFriendsCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Friends_OnQueryFriendsCallback, const EOS_Friends_QueryFriendsCallbackInfo* Data);



/**
 * EOS_Friends_SendInvite is used to start an asynchronous operation to send a friendship invitation from a local user to a target user.
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_Friends_SendInvite API. */
#define EOS_FRIENDS_SENDINVITE_API_LATEST 1

/**
 * Input parameters for the EOS_Friends_SendInvite Function.
 */
EOS_STRUCT(EOS_Friends_SendInviteOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user who is sending the friendship invitation */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who is receiving the friendship invitation */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Output parameters for the EOS_Friends_SendInvite API.
 */
EOS_STRUCT(EOS_Friends_SendInviteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned if the invitation was sent, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Friends_SendInvite */
	void* ClientData;
	/** Account ID of the user who sent the friendship invitation */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user to whom the friendship invitation was sent */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Friends_SendInvite
 * @param Data A EOS_Friends_SendInviteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Friends_OnSendInviteCallback, const EOS_Friends_SendInviteCallbackInfo* Data);



/**
 * EOS_Friends_AcceptInvite is used to start an asynchronous operation to accept a friendship invitation from another user.
 * The following types are used to work with the API.
 */

/** The most recent version of the EOS_Friends_AcceptInvite API. */
#define EOS_FRIENDS_ACCEPTINVITE_API_LATEST 1

/**
 * Input parameters for the EOS_Friends_AcceptInvite Function.
 */
EOS_STRUCT(EOS_Friends_AcceptInviteOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user who is accepting the friendship invitation */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who sent the local user a friendship invitation */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Output parameters for the EOS_Friends_AcceptInvite Function.
 */
EOS_STRUCT(EOS_Friends_AcceptInviteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned if an invite was accepted, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into to EOS_Friends_AcceptInvite */
	void* ClientData;
	/** Account ID of the user who is accepting the friendship invitation */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who sent the local user a friendship invitation */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Friends_AcceptInvite
 * @param Data A EOS_Friends_AcceptInviteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Friends_OnAcceptInviteCallback, const EOS_Friends_AcceptInviteCallbackInfo* Data);



/**
 * EOS_Friends_RejectInvite is used to start an asynchronous operation to reject a friendship invitation from another user.
 * The following types are used to work with the API.
 */

/** The most recent version of the EOS_Friends_RejectInvite API. */
#define EOS_FRIENDS_REJECTINVITE_API_LATEST 1

/**
 * Input parameters for the EOS_Friends_RejectInvite Function.
 */
EOS_STRUCT(EOS_Friends_RejectInviteOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user who is rejecting a friendship invitation */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who sent the friendship invitation */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Output parameters for the EOS_Friends_RejectInvite Function.
 */
EOS_STRUCT(EOS_Friends_RejectInviteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned if an invite was accepted, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into to EOS_Friends_RejectInvite */
	void* ClientData;
	/** Account ID of the user who is rejecting a friendship invitation */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who sent the friendship invitation */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Friends_RejectInvite
 * @param Data A EOS_Friends_RejectInviteCallbackInfo containing output information and the result.
 */
EOS_DECLARE_CALLBACK(EOS_Friends_OnRejectInviteCallback, const EOS_Friends_RejectInviteCallbackInfo* Data);



/**
 * EOS_Friends_DeleteFriend is used to start an asynchronous operation to delete a friend from the friend list.
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_Friends_DeleteFriend API. */
#define EOS_FRIENDS_DELETEFRIEND_API_LATEST 1

/**
 * Input parameters for the EOS_Friends_DeleteFriend Function.
 */
EOS_STRUCT(EOS_Friends_DeleteFriendOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user who is deleting a friendship. */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who is being removed from the friends list. */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Output parameters for the EOS_Friends_DeleteFriend Function.
 */
EOS_STRUCT(EOS_Friends_DeleteFriendCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned if an invite was accepted, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into to EOS_Friends_DeleteFriend */
	void* ClientData;
	/** Account ID of the user who is deleting a friendship. */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user who is being removed from the friends list. */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Friends_DeleteFriend
 * @param Data A EOS_Friends_DeleteFriendCallbackInfo containing output information and the result.
 */
EOS_DECLARE_CALLBACK(EOS_Friends_OnDeleteFriendCallback, const EOS_Friends_DeleteFriendCallbackInfo* Data);



/**
 * EOS_Friends_GetFriendsCount is used to immediately retrieve the number of cached friendships. 
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_Friends_GetFriendsCount API. */
#define EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST 1


/**
 * Input parameters for the EOS_Friends_GetFriendsCount Function.
 */
EOS_STRUCT(EOS_Friends_GetFriendsCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user whose friendships should be counted */
	EOS_EpicAccountId LocalUserId;
));


/**
 * EOS_Friends_GetFriendAtIndex is used to immediately retrieve the account ID of another user who has a friendship (or pending friendship).
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_Friends_GetFriendAtIndex API. */
#define EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_Friends_GetFriendAtIndex Function.
 */
EOS_STRUCT(EOS_Friends_GetFriendAtIndexOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the user whose friend list is being queried */
	EOS_EpicAccountId LocalUserId;
	/** Index into the friend list. This value must be between 0 and EOS_Friends_GetFriendsCount-1 inclusively. */
	int32_t Index;
));



/**
 * EOS_Friends_GetStatus is used to immediately retrieve the friendship status between two users.
 * The following types are used to work with the API.
 */

 /** The most recent version of the EOS_Friends_GetStatus API. */
#define EOS_FRIENDS_GETSTATUS_API_LATEST 1

/**
 * An enumeration of the different friendship statuses.
 */
EOS_ENUM(EOS_EFriendsStatus,
	/** The two accounts have no friendship status */
	EOS_FS_NotFriends = 0,
	/** The local account has sent a friend invite to the other account */
	EOS_FS_InviteSent = 1,
	/** The other account has sent a friend invite to the local account */
	EOS_FS_InviteReceived = 2,
	/** The accounts have accepted friendship */
	EOS_FS_Friends = 3
);

/**
 * Input parameters for the EOS_Friends_GetStatus Function.
 */
EOS_STRUCT(EOS_Friends_GetStatusOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Account ID of the local, logged in user */
	EOS_EpicAccountId LocalUserId;
	/** Account ID of the user whose friendship status with the local user is being queried */
	EOS_EpicAccountId TargetUserId;
));



/**
 * EOS_Friends_AddNotifyFriendsUpdate is used to setup notification to receive any friend status updates.
 * The following types are used to work with the API.
 */

/** The most recent version of the EOS_Friends_AddNotifyFriendsUpdate API. */
#define EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST 1

/** Input parameters for the EOS_Friends_AddNotifyFriendsUpdate Function.  */
EOS_STRUCT(EOS_Friends_AddNotifyFriendsUpdateOptions, (
	/** API Version of the structure */
	int32_t ApiVersion;
));

/**
 * Structure containing information about a friend status update.
 */
EOS_STRUCT(EOS_Friends_OnFriendsUpdateInfo, (
	/** Client-specified data passed into EOS_Friends_AddNotifyFriendsUpdate */
	void* ClientData;
	/** The local user who is being notified of the friends update. */
	EOS_EpicAccountId LocalUserId;
	/** The target user whose status is being updated. */
	EOS_EpicAccountId TargetUserId;
	/** The previous status of the friend. */
	EOS_EFriendsStatus PreviousStatus;
	/** The current status of the friend. */
	EOS_EFriendsStatus CurrentStatus;
));

/**
 * Callback for information related to a friend status update.
 */
EOS_DECLARE_CALLBACK(EOS_Friends_OnFriendsUpdateCallback, const EOS_Friends_OnFriendsUpdateInfo* Data);


#pragma pack(pop)
