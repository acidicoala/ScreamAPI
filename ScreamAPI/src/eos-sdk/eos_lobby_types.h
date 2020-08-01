// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

/** Handle to the lobby interface */
EXTERN_C typedef struct EOS_LobbyHandle* EOS_HLobby;
/** Handle to a lobby modification object */
EXTERN_C typedef struct EOS_LobbyModificationHandle* EOS_HLobbyModification;
/** Handle to a single lobby */
EXTERN_C typedef struct EOS_LobbyDetailsHandle* EOS_HLobbyDetails;
/** Handle to the calls responsible for creating a search object */
EXTERN_C typedef struct EOS_LobbySearchHandle* EOS_HLobbySearch;

EOS_DECLARE_FUNC(void) EOS_LobbyModification_Release(EOS_HLobbyModification LobbyModificationHandle);

/**
 * Release the memory associated with a single lobby. This must be called on data retrieved from EOS_LobbySearch_CopySearchResultByIndex.
 *
 * @param LobbyHandle - The lobby handle to release
 *
 * @see EOS_LobbySearch_CopySearchResultByIndex
 */
EOS_DECLARE_FUNC(void) EOS_LobbyDetails_Release(EOS_HLobbyDetails LobbyHandle);

/**
 * Release the memory associated with a lobby search. This must be called on data retrieved from EOS_Lobby_CreateLobbySearch.
 *
 * @param LobbySearchHandle - The lobby search handle to release
 *
 * @see EOS_Lobby_CreateLobbySearch
 */
EOS_DECLARE_FUNC(void) EOS_LobbySearch_Release(EOS_HLobbySearch LobbySearchHandle);

/** All lobbies are referenced by a unique lobby id */
EXTERN_C typedef const char* EOS_LobbyId;

#define EOS_LOBBY_MAX_LOBBIES 4
#define EOS_LOBBY_MAX_LOBBY_MEMBERS 64
#define EOS_LOBBY_MAX_SEARCH_RESULTS 200

/** Maximum number of attributes allowed on the lobby */
#define EOS_LOBBYMODIFICATION_MAX_ATTRIBUTES 64
/** Maximum length of the name of the attribute associated with the lobby */
#define EOS_LOBBYMODIFICATION_MAX_ATTRIBUTE_LENGTH 64

/** Permission level gets more restrictive further down */
EOS_ENUM(EOS_ELobbyPermissionLevel,
	/** Anyone can find this lobby as long as it isn't full */
	EOS_LPL_PUBLICADVERTISED = 0,
	/** Players who have access to presence can see this lobby */
	EOS_LPL_JOINVIAPRESENCE = 1,
	/** Only players with invites registered can see this lobby */
	EOS_LPL_INVITEONLY = 2
);

/** Advertisement properties for a single attribute associated with a lobby */
EOS_ENUM(EOS_ELobbyAttributeVisibility,
	/** Data is visible outside the lobby */
	EOS_LAT_PUBLIC = 0,
	/** Only members in the lobby can see this data */
	EOS_LAT_PRIVATE = 1
);

/** Various types of lobby member updates */
EOS_ENUM(EOS_ELobbyMemberStatus,
	/** The user has joined the lobby */
	EOS_LMS_JOINED = 0,
	/** The user has explicitly left the lobby */
	EOS_LMS_LEFT = 1,
	/** The user has unexpectedly left the lobby */
	EOS_LMS_DISCONNECTED = 2,
	/** The user has been kicked from the lobby */
	EOS_LMS_KICKED = 3,
	/** The user has been promoted to lobby owner */
	EOS_LMS_PROMOTED = 4,
	/** The lobby has been closed and user has been removed */
	EOS_LMS_CLOSED = 5
);

#define EOS_LOBBYDETAILS_INFO_API_LATEST 1

EOS_STRUCT(EOS_LobbyDetails_Info, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby id */
	EOS_LobbyId LobbyId;
	/** Current owner of the lobby */
	EOS_ProductUserId LobbyOwnerUserId;
	/** Permission level of the lobby */
	EOS_ELobbyPermissionLevel PermissionLevel;
	/** Current available space */
	uint32_t AvailableSlots;
	/** Max allowed members in the lobby */
	uint32_t MaxMembers;
	/** Are invites allowed */
	EOS_Bool bAllowInvites;
));

EOS_DECLARE_FUNC(void) EOS_LobbyDetails_Info_Release(EOS_LobbyDetails_Info* LobbyDetailsInfo);

/** The most recent version of the EOS_Lobby_CreateLobby API. */
#define EOS_LOBBY_CREATELOBBY_API_LATEST 2

/**
 * Input parameters for the EOS_Lobby_CreateLobby Function.
 */
EOS_STRUCT(EOS_Lobby_CreateLobbyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Local user creating the lobby */
	EOS_ProductUserId LocalUserId;
	/** Max members allowed in the lobby */
	uint32_t MaxLobbyMembers;
	/** The initial permission level of the lobby */
	EOS_ELobbyPermissionLevel PermissionLevel;
	/** 
	 * If true then this lobby will be used as the one lobby associated with presence.
	 * Only one lobby at a time can have this flag true.
	 */
	EOS_Bool bPresenceEnabled;
));

/**
 * Output parameters for the EOS_Lobby_CreateLobby function.
 */
EOS_STRUCT(EOS_Lobby_CreateLobbyCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_CreateLobby */
	void* ClientData;
	/** Newly created lobby id */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_CreateLobby
 * @param Data A EOS_Lobby_CreateLobby CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnCreateLobbyCallback, const EOS_Lobby_CreateLobbyCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_DestroyLobby API. */
#define EOS_LOBBY_DESTROYLOBBY_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_DestroyLobby Function.
 */
EOS_STRUCT(EOS_Lobby_DestroyLobbyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Local user destroying the lobby, must own the lobby */
	EOS_ProductUserId LocalUserId;
	/** Lobby Id to destroy */
	EOS_LobbyId LobbyId;
));

/**
 * Output parameters for the EOS_Lobby_DestroyLobby function.
 */
EOS_STRUCT(EOS_Lobby_DestroyLobbyCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_DestroyLobby */
	void* ClientData;
	/** Destroyed lobby id */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_DestroyLobby
 * @param Data A EOS_Lobby_DestroyLobby CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnDestroyLobbyCallback, const EOS_Lobby_DestroyLobbyCallbackInfo* Data);


/** The most recent version of the EOS_Lobby_JoinLobby API. */
#define EOS_LOBBY_JOINLOBBY_API_LATEST 2

/**
 * Input parameters for the EOS_Lobby_JoinLobby Function.
 */
EOS_STRUCT(EOS_Lobby_JoinLobbyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby handle to join */
	EOS_HLobbyDetails LobbyDetailsHandle;
	/** Local user joining the lobby */
	EOS_ProductUserId LocalUserId;
	/** 
	 * If true then this lobby will be used as the one lobby associated with presence.
	 * Only one lobby at a time can have this flag true.
	 */
	EOS_Bool bPresenceEnabled;
));

/**
 * Output parameters for the EOS_Lobby_JoinLobby function.
 */
EOS_STRUCT(EOS_Lobby_JoinLobbyCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_JoinLobby */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_JoinLobby
 * @param Data A EOS_Lobby_JoinLobby CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnJoinLobbyCallback, const EOS_Lobby_JoinLobbyCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_LeaveLobby API. */
#define EOS_LOBBY_LEAVELOBBY_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_LeaveLobby Function.
 */
EOS_STRUCT(EOS_Lobby_LeaveLobbyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Local user leaving the lobby */
	EOS_ProductUserId LocalUserId;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/**
 * Output parameters for the EOS_Lobby_LeaveLobby function.
 */
EOS_STRUCT(EOS_Lobby_LeaveLobbyCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_LeaveLobby */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_LeaveLobby
 * @param Data A EOS_Lobby_LeaveLobby CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnLeaveLobbyCallback, const EOS_Lobby_LeaveLobbyCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_UpdateLobbyModification API. */
#define EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_UpdateLobbyModification Function.
 */
EOS_STRUCT(EOS_Lobby_UpdateLobbyModificationOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** The id of the local user making modifications, must be the owner to modify lobby data, but may be a lobby member to modify their own attributes */
	EOS_ProductUserId LocalUserId;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/** The most recent version of the EOS_Lobby_UpdateLobby API. */
#define EOS_LOBBY_UPDATELOBBY_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_UpdateLobby Function.
 */
EOS_STRUCT(EOS_Lobby_UpdateLobbyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Builder handle */
	EOS_HLobbyModification LobbyModificationHandle;
));

/**
 * Output parameters for the EOS_Lobby_UpdateLobby function.
 */
EOS_STRUCT(EOS_Lobby_UpdateLobbyCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_UpdateLobby */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_UpdateLobby
 * @param Data A EOS_Lobby_UpdateLobby CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnUpdateLobbyCallback, const EOS_Lobby_UpdateLobbyCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_PromoteMember API. */
#define EOS_LOBBY_PROMOTEMEMBER_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_PromoteMember Function.
 */
EOS_STRUCT(EOS_Lobby_PromoteMemberOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby id of interest */
	EOS_LobbyId LobbyId;
	/** Local User making the request */
	EOS_ProductUserId LocalUserId;
	/** Member to promote to owner of the lobby */
	EOS_ProductUserId TargetUserId;
));

/**
 * Output parameters for the EOS_Lobby_PromoteMember function.
 */
EOS_STRUCT(EOS_Lobby_PromoteMemberCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_PromoteMember */
	void* ClientData;
	/** Lobby id of interest */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_PromoteMember
 * @param Data A EOS_Lobby_PromoteMember CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnPromoteMemberCallback, const EOS_Lobby_PromoteMemberCallbackInfo* Data);


/** The most recent version of the EOS_Lobby_KickMember API. */
#define EOS_LOBBY_KICKMEMBER_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_KickMember Function.
 */
EOS_STRUCT(EOS_Lobby_KickMemberOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby id of interest */
	EOS_LobbyId LobbyId;
	/** Local User making the request */
	EOS_ProductUserId LocalUserId;
	/** Member to kick from the lobby */
	EOS_ProductUserId TargetUserId;
));

/**
 * Output parameters for the EOS_Lobby_KickMember function.
 */
EOS_STRUCT(EOS_Lobby_KickMemberCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_KickMember */
	void* ClientData;
	/** Lobby id of interest */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_KickMember
 * @param Data A EOS_Lobby_KickMember CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnKickMemberCallback, const EOS_Lobby_KickMemberCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_AddNotifyLobbyUpdateReceived API. */
#define EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Lobby_OnLobbyUpdateReceivedCallback Function.
 */
EOS_STRUCT(EOS_Lobby_LobbyUpdateReceivedCallbackInfo, (
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyUpdateReceived */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for notifications that comes from EOS_Lobby_AddNotifyLobbyUpdateReceived
 *
 * @param Data A EOS_Lobby_LobbyUpdateReceivedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyUpdateReceivedCallback, const EOS_Lobby_LobbyUpdateReceivedCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_AddNotifyLobbyMemberUpdateReceived API. */
#define EOS_LOBBY_ADDNOTIFYLOBBYMEMBERUPDATERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Lobby_OnLobbyMemberUpdateReceivedCallback Function.
 */
EOS_STRUCT(EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo, (
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyMemberUpdateReceived */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
	/** Target user that was affected */
	EOS_ProductUserId TargetUserId;
));

/**
 * Function prototype definition for notifications that comes from EOS_Lobby_AddNotifyLobbyMemberUpdateReceived
 *
 * @param Data A EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyMemberUpdateReceivedCallback, const EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_AddNotifyLobbyMemberStatusReceived API. */
#define EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_AddNotifyLobbyMemberStatusReceived Function.
 */
EOS_STRUCT(EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Lobby_AddNotifyLobbyMemberStatusReceived function.
 */
EOS_STRUCT(EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo, (
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyMemberStatusReceived */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
	/** Target user that was affected */
	EOS_ProductUserId TargetUserId;
	/** Latest status of the user */
	EOS_ELobbyMemberStatus CurrentStatus;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_AddNotifyLobbyMemberStatusReceived
 * @param Data A EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyMemberStatusReceivedCallback, const EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo* Data);

/** Max length of an invite id */
#define EOS_LOBBY_INVITEID_MAX_LENGTH 64

/** The most recent version of the EOS_Lobby_AddNotifyLobbyInviteReceived API. */
#define EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyInviteReceivedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Lobby_OnLobbyInviteReceivedCallback Function.
 */
EOS_STRUCT(EOS_Lobby_LobbyInviteReceivedCallbackInfo, (
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyInviteReceived */
	void* ClientData;
	/** The invite id */
	const char* InviteId;
	/** User that received the invite */
	EOS_ProductUserId LocalUserId;
	/** Target user that sent the invite */
	EOS_ProductUserId TargetUserId;
));

/**
 * Function prototype definition for notifications that comes from EOS_Lobby_AddNotifyLobbyInviteReceived
 *
 * @param Data A EOS_Lobby_LobbyInviteReceivedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyInviteReceivedCallback, const EOS_Lobby_LobbyInviteReceivedCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_AddNotifyLobbyInviteAccepted API. */
#define EOS_LOBBY_ADDNOTIFYLOBBYINVITEACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Lobby_OnLobbyInviteAcceptedCallback Function.
 */
EOS_STRUCT(EOS_Lobby_LobbyInviteAcceptedCallbackInfo, (
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyInviteAccepted */
	void* ClientData;
	/** The invite id */
	const char* InviteId;
	/** User that received the invite */
	EOS_ProductUserId LocalUserId;
	/** Target user that sent the invite */
	EOS_ProductUserId TargetUserId;
));

/**
 * Function prototype definition for notifications that comes from EOS_Lobby_AddNotifyLobbyInviteAccepted
 *
 * @param Data A EOS_Lobby_LobbyInviteAcceptedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnLobbyInviteAcceptedCallback, const EOS_Lobby_LobbyInviteAcceptedCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_AddNotifyJoinGameAccepted API. */
#define EOS_LOBBY_ADDNOTIFYJOINLOBBYACCEPTED_API_LATEST 1
EOS_STRUCT(EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Lobby_OnJoinLobbyAcceptedCallback Function.
 */
EOS_STRUCT(EOS_Lobby_JoinLobbyAcceptedCallbackInfo, (
	/** Context that was passed into EOS_Lobby_AddNotifyJoinLobbyAccepted */
	void* ClientData;
	/** User that initialized the join game */
	EOS_ProductUserId LocalUserId;
	/** 
	 * The UI Event associated with this Join Game event.
	 * This should be used with EOS_Lobby_CopyLobbyDetailsHandleByUiEventId to get a handle to be used
	 * when calling EOS_Lobby_JoinLobby.
	 */
	EOS_UI_EventId UiEventId;
));

/**
 * Function prototype definition for notifications that comes from EOS_Lobby_AddNotifyJoinLobbyAccepted
 *
 * @param Data A EOS_Lobby_JoinLobbyAcceptedCallbackInfo containing the output information and result
 *
 * @note The lobby for the join game must be joined.
 *
 * @see EOS_Lobby_CopyLobbyDetailsHandleByUiEventId
 * @see EOS_Lobby_JoinLobby
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnJoinLobbyAcceptedCallback, const EOS_Lobby_JoinLobbyAcceptedCallbackInfo* Data);


/** The most recent version of the EOS_Lobby_CopyLobbyDetailsHandleByInviteId API. */
#define EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYINVITEID_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_CopyLobbyDetailsHandleByInviteId Function.
 */
EOS_STRUCT(EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby invite id */
	const char* InviteId;
));

/** The most recent version of the EOS_Lobby_CopyLobbyDetailsHandleByUiEventId API. */
#define EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYUIEVENTID_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_CopyLobbyDetailsHandleByUiEventId Function.
 */
EOS_STRUCT(EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** UI Event associated with the session */
	EOS_UI_EventId UiEventId;
));

/** The most recent version of the EOS_Lobby_CreateLobbySearch API. */
#define EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_CreateLobbySearch Function.
 */
EOS_STRUCT(EOS_Lobby_CreateLobbySearchOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Maximum number of results allowed from the search */
	uint32_t MaxResults;
));

/** The most recent version of the EOS_Lobby_SendInvite API. */
#define EOS_LOBBY_SENDINVITE_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_SendInvite Function.
 */
EOS_STRUCT(EOS_Lobby_SendInviteOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
	/** Local user sending the invite */
	EOS_ProductUserId LocalUserId;
	/** Target user receiving the invite */
	EOS_ProductUserId TargetUserId;
));

/**
 * Output parameters for the EOS_Lobby_SendInvite function.
 */
EOS_STRUCT(EOS_Lobby_SendInviteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_SendInvite */
	void* ClientData;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_SendInvite
 * @param Data A EOS_Lobby_SendInvite CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnSendInviteCallback, const EOS_Lobby_SendInviteCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_RejectInvite API. */
#define EOS_LOBBY_REJECTINVITE_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_RejectInvite Function.
 */
EOS_STRUCT(EOS_Lobby_RejectInviteOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** The invite id to reject */
	const char* InviteId;
	/** Local user rejecting the invite */
	EOS_ProductUserId LocalUserId;
));

/**
 * Output parameters for the EOS_Lobby_RejectInvite function.
 */
EOS_STRUCT(EOS_Lobby_RejectInviteCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_RejectInvite */
	void* ClientData;
	/** The invite id to reject */
	const char* InviteId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_RejectInvite
 * @param Data A EOS_Lobby_RejectInvite CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnRejectInviteCallback, const EOS_Lobby_RejectInviteCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_QueryInvites API. */
#define EOS_LOBBY_QUERYINVITES_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_QueryInvites Function.
 */
EOS_STRUCT(EOS_Lobby_QueryInvitesOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Local User Id to query invites */
	EOS_ProductUserId LocalUserId;
));

/**
 * Output parameters for the EOS_Lobby_QueryInvites function.
 */
EOS_STRUCT(EOS_Lobby_QueryInvitesCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Lobby_QueryInvites */
	void* ClientData;
	/** Local User Id that made the request */
	EOS_ProductUserId LocalUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Lobby_QueryInvites
 * @param Data A EOS_Lobby_QueryInvites CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Lobby_OnQueryInvitesCallback, const EOS_Lobby_QueryInvitesCallbackInfo* Data);


/** The most recent version of the EOS_Lobby_GetInviteCount API. */
#define EOS_LOBBY_GETINVITECOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_GetInviteCount Function.
 */
EOS_STRUCT(EOS_Lobby_GetInviteCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Local user that has invites */
	EOS_ProductUserId LocalUserId;
));

/** The most recent version of the EOS_Lobby_GetInviteIdByIndex API. */
#define EOS_LOBBY_GETINVITEIDBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_GetInviteIdByIndex Function.
 */
EOS_STRUCT(EOS_Lobby_GetInviteIdByIndexOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Local user that has invites */
	EOS_ProductUserId LocalUserId;
	/** Index of the invite id to retrieve */
	uint32_t Index;
));

/** The most recent version of the EOS_Lobby_CopyLobbyHandle API. */
#define EOS_LOBBY_COPYLOBBYDETAILSHANDLE_API_LATEST 1

/**
 * Input parameters for the EOS_Lobby_CopyLobbyDetailsHandle Function.
 */
EOS_STRUCT(EOS_Lobby_CopyLobbyDetailsHandleOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** The id of the lobby affected */
	EOS_LobbyId LobbyId;
	/** Local user making the request */
	EOS_ProductUserId LocalUserId;
));

/** Search for lobbies that contain at least this number of members (value is int)  */
#define EOS_LOBBY_SEARCH_MINCURRENTMEMBERS "mincurrentmembers"
/** Search for a match with min free space (value is int) */
#define EOS_LOBBY_SEARCH_MINSLOTSAVAILABLE "minslotsavailable"

/** The most recent version of the EOS_Lobby_AttributeData struct. */
#define EOS_LOBBY_ATTRIBUTEDATA_API_LATEST 1

/**
 * Contains information about lobby and lobby member data
 */
EOS_STRUCT(EOS_Lobby_AttributeData, (
	/** API Version */
	int32_t ApiVersion;
	/** Name of the lobby attribute */
	const char* Key;
	union
	{
		/** Stored as an 8 byte integer */
		int64_t AsInt64;
		/** Stored as a double precision floating point */
		double AsDouble;
		/** Stored as a boolean */
		EOS_Bool AsBool;
		/** Stored as a null terminated UTF8 string */
		const char* AsUtf8;
	} Value;

	/** Type of value stored in the union */
	EOS_ELobbyAttributeType ValueType;
));

/** The most recent version of the EOS_Lobby_Attribute struct. */
#define EOS_LOBBY_ATTRIBUTE_API_LATEST 1

/**
 *  An attribute and its visibility setting stored with a lobby.
 *  Used to store both lobby and lobby member data
 */
EOS_STRUCT(EOS_Lobby_Attribute, (
	/** API Version */
	int32_t ApiVersion;
	/** Key/Value pair describing the attribute */
	EOS_Lobby_AttributeData* Data;
	/** Is this attribute public or private to the lobby and its members */
	EOS_ELobbyAttributeVisibility Visbility;
));

EOS_DECLARE_FUNC(void) EOS_Lobby_Attribute_Release(EOS_Lobby_Attribute* LobbyAttribute);

/** The most recent version of the EOS_LobbyModification_SetPermissionLevel API. */
#define EOS_LOBBYMODIFICATION_SETPERMISSIONLEVEL_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyModification_SetPermissionLevel Function.
 */
EOS_STRUCT(EOS_LobbyModification_SetPermissionLevelOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Permission level of the lobby */
	EOS_ELobbyPermissionLevel PermissionLevel;
));

/** The most recent version of the EOS_LobbyModification_SetMaxMembers API. */
#define EOS_LOBBYMODIFICATION_SETMAXMEMBERS_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyModification_SetMaxMembers Function.
 */
EOS_STRUCT(EOS_LobbyModification_SetMaxMembersOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** New maximum number of lobby members */
	uint32_t MaxMembers;
));


/** The most recent version of the EOS_LobbyModification_AddAttribute API. */
#define EOS_LOBBYMODIFICATION_ADDATTRIBUTE_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyModification_AddAttribute Function.
 */
EOS_STRUCT(EOS_LobbyModification_AddAttributeOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Key/Value pair describing the attribute to add to the lobby */
	const EOS_Lobby_AttributeData* Attribute;
	/** Is this attribute public or private to the lobby and its members */
	EOS_ELobbyAttributeVisibility Visibility;
));


/** The most recent version of the EOS_LobbyModification_RemoveAttribute API. */
#define EOS_LOBBYMODIFICATION_REMOVEATTRIBUTE_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyModification_RemoveAttribute Function.
 */
EOS_STRUCT(EOS_LobbyModification_RemoveAttributeOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Name of the key */
	const char* Key;
));

/** The most recent version of the EOS_LobbyModification_AddMemberAttribute API. */
#define EOS_LOBBYMODIFICATION_ADDMEMBERATTRIBUTE_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyModification_AddMemberAttribute Function.
 */
EOS_STRUCT(EOS_LobbyModification_AddMemberAttributeOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Key/Value pair describing the attribute to add to the lobby member */
	const EOS_Lobby_AttributeData* Attribute;
	/** Is this attribute public or private to the rest of the lobby members */
	EOS_ELobbyAttributeVisibility Visibility;
));

/** The most recent version of the EOS_LobbyModification_RemoveMemberAttribute API. */
#define EOS_LOBBYMODIFICATION_REMOVEMEMBERATTRIBUTE_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyModification_RemoveMemberAttribute Function.
 */
EOS_STRUCT(EOS_LobbyModification_RemoveMemberAttributeOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Name of the key */
	const char* Key;
));

/** The most recent version of the EOS_LobbyDetails_GetLobbyOwner API. */
#define EOS_LOBBYDETAILS_GETLOBBYOWNER_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_GetLobbyOwner Function.
 */
EOS_STRUCT(EOS_LobbyDetails_GetLobbyOwnerOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_LobbyDetails_CopyInfo API. */
#define EOS_LOBBYDETAILS_COPYINFO_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_CopyInfo Function.
 */
EOS_STRUCT(EOS_LobbyDetails_CopyInfoOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));


/** The most recent version of the EOS_LobbyDetails_GetAttributeCount API. */
#define EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_GetAttributeCount Function.
 */
EOS_STRUCT(EOS_LobbyDetails_GetAttributeCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));


/** The most recent version of the EOS_LobbyDetails_CopyAttributeByIndex API. */
#define EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_CopyAttributeByIndex Function.
 */
EOS_STRUCT(EOS_LobbyDetails_CopyAttributeByIndexOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/**
	 * The index of the attribute to retrieve
	 * @see EOS_LobbyDetails_GetAttributeCount
	 */
	uint32_t AttrIndex;
));


/** The most recent version of the EOS_LobbyDetails_CopyAttributeByKey API. */
#define EOS_LOBBYDETAILS_COPYATTRIBUTEBYKEY_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_CopyAttributeByKey Function.
 */
EOS_STRUCT(EOS_LobbyDetails_CopyAttributeByKeyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Name of the attribute */
	const char* AttrKey;
));

/** The most recent version of the EOS_LobbyDetails_GetMemberAttributeCount API. */
#define EOS_LOBBYDETAILS_GETMEMBERATTRIBUTECOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_GetMemberAttributeCount Function.
 */
EOS_STRUCT(EOS_LobbyDetails_GetMemberAttributeCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby member of interest */
	EOS_ProductUserId TargetUserId;
));

/** The most recent version of the EOS_LobbyDetails_CopyMemberAttributeByIndex API. */
#define EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_CopyMemberAttributeByIndex Function.
 */
EOS_STRUCT(EOS_LobbyDetails_CopyMemberAttributeByIndexOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby member of interest */
	EOS_ProductUserId TargetUserId;
	/** Attribute index */
	uint32_t AttrIndex;
));

/** The most recent version of the EOS_LobbyDetails_CopyMemberAttributeByKey API. */
#define EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYKEY_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_CopyMemberAttributeByKey Function.
 */
EOS_STRUCT(EOS_LobbyDetails_CopyMemberAttributeByKeyOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Lobby member of interest */
	EOS_ProductUserId TargetUserId;
	/** Name of the attribute */
	const char* AttrKey;
));

/** The most recent version of the EOS_LobbyDetails_GetMemberCount API. */
#define EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_GetMemberCount Function.
 */
EOS_STRUCT(EOS_LobbyDetails_GetMemberCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_LobbyDetails_GetMemberByIndex API. */
#define EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_LobbyDetails_GetMemberByIndex Function.
 */
EOS_STRUCT(EOS_LobbyDetails_GetMemberByIndexOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Index of the member to retrieve */
	uint32_t MemberIndex;
));

/** The most recent version of the EOS_LobbySearch_Find API. */
#define EOS_LOBBYSEARCH_FIND_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_Find Function.
 */
EOS_STRUCT(EOS_LobbySearch_FindOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** User making the search request */
	EOS_ProductUserId LocalUserId;
));

/**
 * Output parameters for the EOS_LobbySearch_Find function.
 */
EOS_STRUCT(EOS_LobbySearch_FindCallbackInfo, (
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_LobbySearch_Find */
	void* ClientData;
));

/**
 * Function prototype definition for callbacks passed to EOS_LobbySearch_Find
 * @param Data A EOS_LobbySearch_Find CallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_LobbySearch_OnFindCallback, const EOS_LobbySearch_FindCallbackInfo* Data);

/** The most recent version of the EOS_Lobby_SetLobbyId API. */
#define EOS_LOBBYSEARCH_SETLOBBYID_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_SetLobbyId Function.
 */
EOS_STRUCT(EOS_LobbySearch_SetLobbyIdOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** The id of the lobby to set */
	EOS_LobbyId LobbyId;
));

/** The most recent version of the EOS_Lobby_SetTargetUserId API. */
#define EOS_LOBBYSEARCH_SETTARGETUSERID_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_SetTargetUserId Function.
 */
EOS_STRUCT(EOS_LobbySearch_SetTargetUserIdOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Search lobbies for given user, returning any lobbies where this user is currently registered */
	EOS_ProductUserId TargetUserId;
));

/** The most recent version of the EOS_LobbySearch_SetParameter API. */
#define EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_SetParameter Function.
 */
EOS_STRUCT(EOS_LobbySearch_SetParameterOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Search parameter describing a key and a value to compare */
	const EOS_Lobby_AttributeData* Parameter;
	/** The type of comparison to make against the search parameter */
	EOS_EComparisonOp ComparisonOp;
));

/** The most recent version of the EOS_LobbySearch_RemoveParameter API. */
#define EOS_LOBBYSEARCH_REMOVEPARAMETER_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_RemoveParameter Function.
 */
EOS_STRUCT(EOS_LobbySearch_RemoveParameterOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Search parameter key to remove from the search */
	const char* Key;
	/** Search comparison operation associated with the key to remove */
	EOS_EComparisonOp ComparisonOp;
));

/** The most recent version of the EOS_LobbySearch_SetMaxResults API. */
#define EOS_LOBBYSEARCH_SETMAXRESULTS_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_SetMaxResults Function.
 */
EOS_STRUCT(EOS_LobbySearch_SetMaxResultsOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/** Maximum number of search results to return from the query */
	uint32_t MaxResults;
));

/** The most recent version of the EOS_LobbySearch_GetSearchResultCount API. */
#define EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_GetSearchResultCount Function.
 */
EOS_STRUCT(EOS_LobbySearch_GetSearchResultCountOptions, (
	/** Version of the API */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_LobbySearch_CopySearchResultByIndex API. */
#define EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_LobbySearch_CopySearchResultByIndex Function.
 */
EOS_STRUCT(EOS_LobbySearch_CopySearchResultByIndexOptions, (
	/** Version of the API */
	int32_t ApiVersion;
	/**
	 * The index of the lobby to retrieve within the completed search query
	 * @see EOS_LobbySearch_GetSearchResultCount
	 */
	uint32_t LobbyIndex;
));

#pragma pack(pop)
