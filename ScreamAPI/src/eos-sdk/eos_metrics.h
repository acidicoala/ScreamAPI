// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_metrics_types.h"

/**
 * Logs the start of a new game session for a local player.
 *
 * The game client should call this function whenever it joins into a new multiplayer, peer-to-peer or single player game session.
 * Each call to BeginPlayerSession must be matched with a corresponding call to EndPlayerSession.
 *
 * @param Options Structure containing the local player's game account and the game session information.
 *
 * @return Returns EOS_Success on success, or an error code if the input parameters are invalid or an active session for the player already exists.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_BeginPlayerSession(EOS_HMetrics Handle, const EOS_Metrics_BeginPlayerSessionOptions* Options);

/**
 * Logs the end of a game session for a local player.
 *
 * Call once when the game client leaves the active game session.
 * Each call to BeginPlayerSession must be matched with a corresponding call to EndPlayerSession.
 *
 * @param Options Structure containing the account id of the player whose session to end.
 *
 * @return Returns EOS_Success on success, or an error code if the input parameters are invalid or there was no active session for the player.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_Metrics_EndPlayerSession(EOS_HMetrics Handle, const EOS_Metrics_EndPlayerSessionOptions* Options);
