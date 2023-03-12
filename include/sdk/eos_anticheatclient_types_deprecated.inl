// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

/*
 * This file contains the deprecated types for EOS AntiCheat Client. In a future version, these types will be removed.
 */

#pragma pack(push, 8)

#define EOS_ANTICHEATCLIENT_POLLSTATUS_API_LATEST 1
EOS_STRUCT(EOS_AntiCheatClient_PollStatusOptions, (
	/** API Version: Set this to EOS_ANTICHEATCLIENT_POLLSTATUS_API_LATEST. */
	int32_t ApiVersion;
	/** The size of OutMessage in bytes. Recommended size is 256 bytes. */
	uint32_t OutMessageLength;
));

#pragma pack(pop)
