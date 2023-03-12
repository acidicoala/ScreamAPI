// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_integratedplatform_types.h"

/**
 * To add integrated platforms, you must call EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer to create an integrated platform options container. To modify that handle, call
 * EOS_IntegratedPlatformOptionsContainer_* methods. Once you are finished, call EOS_Platform_Create with your handle. You must then release your integrated platform options container
 * handle by calling EOS_IntegratedPlatformOptionsContainer_Release.
 */

/**
 * Adds an integrated platform options to the container.
 *
 * @param Options Object containing properties related to setting a user's Status
 * @return Success if modification was added successfully, otherwise an error code related to the problem
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatformOptionsContainer_Add(EOS_HIntegratedPlatformOptionsContainer Handle, const EOS_IntegratedPlatformOptionsContainer_AddOptions* InOptions);
