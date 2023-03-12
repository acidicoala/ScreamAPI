// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"
#include "eos_integratedplatform_types.h"

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_PlatformHandle* EOS_HPlatform;

/** Client credentials. */
EOS_STRUCT(EOS_Platform_ClientCredentials, (
	/** Client ID of the service permissions entry. Set to NULL if no service permissions are used. */
	const char* ClientId;
	/** Client secret for accessing the set of permissions. Set to NULL if no service permissions are used. */
	const char* ClientSecret;
));

/** The most recent version of the EOS_Platform_RTCOptions API. */
#define EOS_PLATFORM_RTCOPTIONS_API_LATEST 1

/** Platform RTC options. */
EOS_STRUCT(EOS_Platform_RTCOptions, (
	/** API Version: Set this to EOS_PLATFORM_RTCOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * This field is for platform specific initialization if any.
	 *
	 * If provided then the structure will be located in <System>/eos_<System>.h.
	 * The structure will be named EOS_<System>_RTCOptions.
	 */
	void* PlatformSpecificOptions;
));

#define EOS_COUNTRYCODE_MAX_LENGTH 4
#define EOS_COUNTRYCODE_MAX_BUFFER_LEN (EOS_COUNTRYCODE_MAX_LENGTH + 1)
#define EOS_LOCALECODE_MAX_LENGTH 9
#define EOS_LOCALECODE_MAX_BUFFER_LEN (EOS_LOCALECODE_MAX_LENGTH + 1)

#define EOS_PLATFORM_OPTIONS_API_LATEST 12

/* Platform Creation Flags used in EOS_Platform_Create */

/** A bit that indicates the SDK is being loaded in a game editor, like Unity or UE4 Play-in-Editor */
#define EOS_PF_LOADING_IN_EDITOR				0x00001
/** A bit that indicates the SDK should skip initialization of the overlay, which is used by the in-app purchase flow and social overlay. This bit is implied by EOS_PF_LOADING_IN_EDITOR */
#define EOS_PF_DISABLE_OVERLAY					0x00002
/** A bit that indicates the SDK should skip initialization of the social overlay, which provides an overlay UI for social features. This bit is implied by EOS_PF_LOADING_IN_EDITOR or EOS_PF_DISABLE_OVERLAY */
#define EOS_PF_DISABLE_SOCIAL_OVERLAY			0x00004
/** A reserved bit */
#define EOS_PF_RESERVED1						0x00008
/** A bit that indicates your game would like to opt-in to experimental Direct3D 9 support for the overlay. This flag is only relevant on Windows */
#define EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9		0x00010
/** A bit that indicates your game would like to opt-in to experimental Direct3D 10 support for the overlay. This flag is only relevant on Windows */
#define EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10		0x00020
/** A bit that indicates your game would like to opt-in to experimental OpenGL support for the overlay. This flag is only relevant on Windows */
#define EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL	0x00040

/** Platform options for EOS_Platform_Create. */
EOS_STRUCT(EOS_Platform_Options, (
	/** API Version: Set this to EOS_PLATFORM_OPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The product ID for the running application, found on the dev portal */
	const char* ProductId;
	/** The sandbox ID for the running application, found on the dev portal */
	const char* SandboxId;
	/** Set of service permissions associated with the running application */
	EOS_Platform_ClientCredentials ClientCredentials;
	/** Set this to EOS_FALSE if the application is running as a client with a local user, otherwise set to EOS_TRUE (e.g. for a dedicated game server) */
	EOS_Bool bIsServer;
	/** Used by Player Data Storage and Title Storage. Must be null initialized if unused. 256-bit Encryption Key for file encryption in hexadecimal format (64 hex chars)*/
	const char* EncryptionKey;
	/** The override country code to use for the logged in user. (EOS_COUNTRYCODE_MAX_LENGTH)*/
	const char* OverrideCountryCode;
	/** The override locale code to use for the logged in user. This follows ISO 639. (EOS_LOCALECODE_MAX_LENGTH)*/
	const char* OverrideLocaleCode;
	/** The deployment ID for the running application, found on the dev portal */
	const char* DeploymentId;
	/** Platform creation flags, e.g. EOS_PF_LOADING_IN_EDITOR. This is a bitwise-or union of the defined flags. */
	uint64_t Flags;
	/** Used by Player Data Storage and Title Storage. Must be null initialized if unused. Cache directory path. Absolute path to the folder that is going to be used for caching temporary data. The path is created if it's missing. */
	const char* CacheDirectory;
	/**
	 * A budget, measured in milliseconds, for EOS_Platform_Tick to do its work. When the budget is met or exceeded (or if no work is available), EOS_Platform_Tick will return.
	 * This allows your game to amortize the cost of SDK work across multiple frames in the event that a lot of work is queued for processing.
	 * Zero is interpreted as "perform all available work".
	 */
	uint32_t TickBudgetInMilliseconds;
	/** RTC options. Setting to NULL will disable RTC features (e.g. voice) */
	const EOS_Platform_RTCOptions* RTCOptions;
	/**
	 * A handle that contains all the options for setting up integrated platforms.
	 * When set to NULL, the default integrated platform behavior for the host platform will be used.
	 */
	EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle;
));

/**
 * All possible states of the application
 */
EOS_ENUM(EOS_EApplicationStatus,
	/** Background constrained */
	EOS_AS_BackgroundConstrained = 0,
	/** Background unconstrained */
	EOS_AS_BackgroundUnconstrained = 1,
	/** Background suspended */
	EOS_AS_BackgroundSuspended = 2,
	/** Foreground */
	EOS_AS_Foreground = 3
);

/**
 * All possible states of the network
 */
EOS_ENUM(EOS_ENetworkStatus,
	/** Network cannot be used. */
	EOS_NS_Disabled = 0,
	/** We may not be connected to the internet. The network can still be used, but is expected to probably fail. */
	EOS_NS_Offline = 1,
	/** We think we're connected to the internet. */
	EOS_NS_Online = 2
);

/**
 * Possible statuses for the availability of desktop crossplay functionality.
 *
 * @see EOS_Platform_GetDesktopCrossplayStatus
 */
EOS_ENUM(EOS_EDesktopCrossplayStatus,
	/**
	 * Desktop crossplay is ready to use.
	 */
	EOS_DCS_OK = 0,
	/**
	 * The application was not launched through the Bootstrapper.
	 */
	EOS_DCS_ApplicationNotBootstrapped = 1,
	/**
	 * The redistributable service is not installed.
	 */
	EOS_DCS_ServiceNotInstalled = 2,
	/**
	 * The service failed to start.
	 */
	EOS_DCS_ServiceStartFailed = 3,
	/**
	 * The service was started successfully, but is no longer running in the background, for an unknown reason.
	 */
	EOS_DCS_ServiceNotRunning = 4,
	/**
	 * The application has explicitly disabled the overlay through SDK initialization flags.
	 */
	EOS_DCS_OverlayDisabled = 5,
	/**
	 * The overlay is not installed.
	 *
	 * As the overlay is automatically installed and kept up-to-date by the redistributable service,
	 * this indicates that the user may have separately manually removed the installed overlay files.
	 */
	EOS_DCS_OverlayNotInstalled = 6,
	/**
	 * The overlay was not loaded due to failing trust check on the digital signature of the file on disk.
	 *
	 * This error typically indicates one of the following root causes:
	 * - The Operating System's local certificate store is out of date.
	 * - The local system clock has skewed and is in the wrong time.
	 * - The file has been tampered with.
	 * - The file trust check timed out, either due to an issue with the local system or network connectivity.
	 *
	 * The first troubleshooting steps should be to check for any available Operating System updates,
	 * for example using the Windows Update, as well as verifying that the system time is correctly set.
	 */
	EOS_DCS_OverlayTrustCheckFailed = 7,
	/**
	 * The overlay failed to load.
	 */
	EOS_DCS_OverlayLoadFailed = 8
);

/** The most recent version of the EOS_Platform_GetDesktopCrossplayStatus API. */
#define EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST 1

/**
 * Input parameters for the EOS_Platform_GetDesktopCrossplayStatus function.
 */
EOS_STRUCT(EOS_Platform_GetDesktopCrossplayStatusOptions, (
	/** API Version: Set this to EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Platform_GetDesktopCrossplayStatus function.
 */
EOS_STRUCT(EOS_Platform_GetDesktopCrossplayStatusInfo, (
	/**
	 * Status for the availability of desktop crossplay functionality.
	 *
	 * It is recommended to include this value in application logs, and as as part of
	 * any player-facing error screens to help troubleshooting possible issues.
	 */
	EOS_EDesktopCrossplayStatus Status;
	/**
	 * This field is set when the Status is EOS_DCS_ServiceStartFailed.
	 *
	 * Possible values for this field are not documented. However, it is recommended
	 * to be also included in application logs, and as part of any player-facing
	 * error screens.
	 */
	int32_t ServiceInitResult;
));

#pragma pack(pop)
