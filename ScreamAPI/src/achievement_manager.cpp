#include "pch.h"
#include "achievement_manager.h"
#include "ScreamAPI.h"
#include "util.h"
#include "eos-sdk/eos_achievements.h"
#include "Overlay.h"
#include <future>

using namespace Util;

namespace AchievementManager{

Achievements achievements;

void printAchievementDefinition(EOS_Achievements_DefinitionV2* definition){
	if(definition == nullptr){
		Logger::ach("Invalid Achievement Definition");
	} else {
		std::stringstream ss;
		// We are using stream stream since rapid opening and closing log file adversely impacts the performance
		ss
			<< "[Achievement Definition]\n"
			<< "\t\t\t""AchievementId: " << definition->AchievementId << "\n"
			<< "\t\t\t""IsHidden: " << definition->bIsHidden << "\n"
			<< "\t\t\t""FlavorText: " << definition->FlavorText << "\n"
			<< "\t\t\t""LockedDescription: " << definition->LockedDescription << "\n"
			<< "\t\t\t""LockedDisplayName: " << definition->LockedDisplayName << "\n"
			<< "\t\t\t""LockedIconURL: " << definition->LockedIconURL << "\n"
			<< "\t\t\t""UnlockedDescription: " << definition->UnlockedDescription << "\n"
			<< "\t\t\t""UnlockedDisplayName: " << definition->UnlockedDisplayName << "\n"
			<< "\t\t\t""UnlockedIconURL: " << definition->UnlockedIconURL << "\n"
			<< "\t\t\t""StatThresholdsCount: " << definition->StatThresholdsCount << "\n";

		for(unsigned int i = 0; i < definition->StatThresholdsCount; i++){
			ss
				<< "\t\t\t\t""[StatThreshold] "
				<< "Name: " << definition->StatThresholds[i].Name << "; "
				<< "Threshold: " << definition->StatThresholds[i].Threshold;
		}
		// Escape the whole string because of % characters in URLs
		Logger::ach("%s", ss.str().c_str());
	}
}

void printPlayerAchievement(EOS_Achievements_PlayerAchievement* achievement){
	if(achievement == nullptr){
		Logger::ach("Invalid Player Achievement");
	} else{
		std::stringstream ss;
		// We are using stream stream since rapid opening and closing log file adversely impacts the performance
		ss
			<< "[Player Achievement]\n"
			<< "\t\t\t""AchievementId: " << achievement->AchievementId << "\n"
			<< "\t\t\t""Description: " << achievement->Description << "\n"
			<< "\t\t\t""DisplayName: " << achievement->DisplayName << "\n"
			<< "\t\t\t""FlavorText: " << achievement->FlavorText << "\n"
			<< "\t\t\t""IconURL: " << achievement->IconURL << "\n"
			<< "\t\t\t""Progress: " << achievement->Progress << "\n"
			<< "\t\t\t""StatInfoCount: " << achievement->StatInfoCount << "\n";

		for(int i = 0; i < achievement->StatInfoCount; i++){
			ss
				<< "\t\t\t\t""[StatInfo] "
				<< "Name: " << achievement->StatInfo[i].Name << "; "
				<< "CurrentValue: " << achievement->StatInfo[i].CurrentValue << "; "
				<< "ThresholdValue: " << achievement->StatInfo[i].ThresholdValue;
		}
		// Escape the whole string because of % characters in URLs
		Logger::ach("%s", ss.str().c_str());
	}
}

/**
 * Searches the overlay achievements vector and executes the callback function
 * on the achievment with the given id, if it exists. Otherwise prints error log
 */
void findAchievement(const char* achievementID, std::function<void(Overlay_Achievement&)> callback){
	// Find the corresponding achievement in our achievement array
	for(auto& achievement : achievements){
		// Compare by AchievementId
		if(!strcmp(achievement.AchievementId, achievementID)){
			// Found it
			callback(achievement);
			return;
		}
	}

	Logger::error("Could not find achievement with id: %s", achievementID);
}

/**
 * A callback function that is executed when an Unlock button is pressed in the Overlay
 */
void unlockAchievement(Overlay_Achievement* achievement){
	achievement->UnlockState = UnlockState::Unlocking;

	EOS_Achievements_UnlockAchievementsOptions Options = {
		EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST,
		getProductUserId(),
		&achievement->AchievementId,
		1
	};

	EOS_Achievements_UnlockAchievements(getHAchievements(), &Options, achievement,
		[/* Callback */](const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data){
		auto achievement = (Overlay_Achievement*)Data->ClientData;


		if(Data->ResultCode == EOS_EResult::EOS_Success){
			Logger::info("Successfully unlocked the achievement: %s", achievement->AchievementId);
		} else { // Abort if something went wrong
			achievement->UnlockState = UnlockState::Locked;
			Logger::error("Failed to unlock the achievement: %s. Error string: %s",
				achievement->AchievementId,
				EOS_EResult_ToString(Data->ResultCode));
		}
	});
}

void EOS_CALL queryPlayerAchievementsComplete(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data){
	Logger::debug("queryPlayerAchievementsComplete");

	// Abort if something went wrong
	if(Data->ResultCode != EOS_EResult::EOS_Success){
		Logger::error("Failed to query player achievements. Result string: %s",
			EOS_EResult_ToString(Data->ResultCode));
		return;
	}

	// Get number of player achievements
	static EOS_Achievements_GetPlayerAchievementCountOptions GetCountOptions{
		EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST,
		getProductUserId()
	};
	auto playerAchievementsCount = EOS_Achievements_GetPlayerAchievementCount(getHAchievements(), &GetCountOptions);
	// Iterate over queried player achievements and update our own structs
	for(unsigned int i = 0; i < playerAchievementsCount; i++){
		EOS_Achievements_CopyPlayerAchievementByIndexOptions CopyAchievementOptions{
			EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST,
			getProductUserId(),
			i
		};
		EOS_Achievements_PlayerAchievement* OutAchievement;
		auto result = EOS_Achievements_CopyPlayerAchievementByIndex(
			getHAchievements(),
			&CopyAchievementOptions,
			&OutAchievement
		);
		if(result != EOS_EResult::EOS_Success){
			Logger::error("Failed to copy player achievement by index %d. "
				"Result string: %s", i, EOS_EResult_ToString(result));
			continue;
		}

		printPlayerAchievement(OutAchievement);

		// Update our achievement array if this achievement is unlocked
		if(OutAchievement->UnlockTime != -1){
			findAchievement(OutAchievement->AchievementId, [](Overlay_Achievement& achievement){
				achievement.UnlockState = UnlockState::Unlocked;
			});
		}

		EOS_Achievements_PlayerAchievement_Release(OutAchievement);
	}

	// Initialize Overlay
	Overlay::Init(ScreamAPI::thisDLL, &achievements, unlockAchievement);
}

/**
 * A callback function that is executed when EOS has done querying achievement definitions at out request.
 *
 * @see queryAchievementDefinitions
 * @see EOS_Achievements_QueryDefinitions
 */
void EOS_CALL queryDefinitionsComplete(const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data){
	Logger::debug("queryDefinitionsComplete");

	// Abort if something went wrong
	if(Data->ResultCode != EOS_EResult::EOS_Success){
		Logger::error("Failed to query achievement definitions. Result string: %s",
			EOS_EResult_ToString(Data->ResultCode));
		return;
	}

	// Get number of achievement definitions
	static EOS_Achievements_GetAchievementDefinitionCountOptions GetCountOptions{
		EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST
	};
	auto achievementDefinitionCount = EOS_Achievements_GetAchievementDefinitionCount(getHAchievements(), &GetCountOptions);
	// Iterate over queried achievement definitions and save them to our own structs
	for(uint32_t i = 0; i < achievementDefinitionCount; i++){
		static bool useDeprecated = false;

		if(!useDeprecated){
			try{
				EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions DefinitionOptions{
					EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYINDEX_API_LATEST,
					i
				};
				EOS_Achievements_DefinitionV2* OutDefinition;
				auto result = EOS_Achievements_CopyAchievementDefinitionV2ByIndex(getHAchievements(), &DefinitionOptions, &OutDefinition);
				if(result != EOS_EResult::EOS_Success){
					Logger::error("Failed to copy achievement definition by index %d. "
						"Result string: %s", i, EOS_EResult_ToString(result));
					continue;
				}

				printAchievementDefinition(OutDefinition);

				achievements.push_back(
					{
						copy_c_string(OutDefinition->AchievementId),
						(bool)OutDefinition->bIsHidden,
						UnlockState::Locked,
						copy_c_string(OutDefinition->UnlockedDescription),
						copy_c_string(OutDefinition->UnlockedDisplayName),
						copy_c_string(OutDefinition->UnlockedIconURL),
						nullptr
					}
				);

				EOS_Achievements_DefinitionV2_Release(OutDefinition);
				continue;
			} catch(ScreamAPI::FunctionNotFoundException){
				useDeprecated = true;
			}
		}

		EOS_Achievements_CopyAchievementDefinitionByIndexOptions DefinitionOptions{
			EOS_ACHIEVEMENTS_COPYDEFINITIONBYINDEX_API_LATEST,
			i
		};
		EOS_Achievements_Definition* OutDefinition;
		auto result = EOS_Achievements_CopyAchievementDefinitionByIndex(getHAchievements(), &DefinitionOptions, &OutDefinition);
		if(result != EOS_EResult::EOS_Success){
			Logger::error("Failed to copy (deprecated) achievement definition by index %d. "
				"Result string: %s", i, EOS_EResult_ToString(result));
			continue;
		}

		// TODO: Implement printer for deprecated achievement definitions?
		// printAchievementDefinition(OutDefinition);

		achievements.push_back(
			{
				copy_c_string(OutDefinition->AchievementId),
				(bool)OutDefinition->bIsHidden,
				UnlockState::Locked,
				copy_c_string(OutDefinition->CompletionDescription),
				copy_c_string(OutDefinition->DisplayName),
				copy_c_string(OutDefinition->UnlockedIconId),
				nullptr
			}
		);

		EOS_Achievements_Definition_Release(OutDefinition);
	}

	// Query player achievements to update the ones that are already unlocked
	EOS_Achievements_QueryPlayerAchievementsOptions QueryAchievementsOptions = {
			EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST,
			getProductUserId()
	};

	EOS_Achievements_QueryPlayerAchievements(
		getHAchievements(),
		&QueryAchievementsOptions,
		nullptr,
		queryPlayerAchievementsComplete
	);
}

void queryAchievementDefinitions(){
	// Query achievement definitions
	EOS_Achievements_QueryDefinitionsOptions QueryDefinitionsOptions = {
			1, // Use legacy API, since older DLLs will throw EOS_IncompatibleVersion error
			getProductUserId(),
			getEpicAccountId(),
			nullptr,
			0
	};

	EOS_Achievements_QueryDefinitions(
		getHAchievements(),
		&QueryDefinitionsOptions,
		nullptr,
		queryDefinitionsComplete
	);
}

/**
 * Subscribe to AchievementsUnlocked events so that we can update
 * the state of achievements in our own list.
 */
void subscribeToAchievementNotifications(){
	static bool useDeprecated = false;

	if(!useDeprecated){
		try{
			EOS_Achievements_AddNotifyAchievementsUnlockedV2Options NotifyOptions = {
				EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKEDV2_API_LATEST
			};

			EOS_Achievements_AddNotifyAchievementsUnlockedV2(getHAchievements(), &NotifyOptions, nullptr,
				[](const EOS_Achievements_OnAchievementsUnlockedCallbackV2Info* Data){
				findAchievement(Data->AchievementId, [](Overlay_Achievement& achievement){
					achievement.UnlockState = UnlockState::Unlocked;
				});
			});

			return;
		} catch(ScreamAPI::FunctionNotFoundException){
			useDeprecated = true;
		}
	}

	EOS_Achievements_AddNotifyAchievementsUnlockedOptions NotifyOptions = {
		EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKED_API_LATEST
	};

	EOS_Achievements_AddNotifyAchievementsUnlocked(getHAchievements(), &NotifyOptions, nullptr,
		[](const EOS_Achievements_OnAchievementsUnlockedCallbackInfo* Data){
		for(uint32_t i = 0; i < Data->AchievementsCount; i++){
			findAchievement(Data->AchievementIds[i], [](Overlay_Achievement& achievement){
				achievement.UnlockState = UnlockState::Unlocked;
			});
		}
	});
}

void init() {
	static bool init = false;

	if(!init && Config::EnableOverlay()) {
		Logger::ovrly("Achievement Manager: Initializing...");
		init = true;
		queryAchievementDefinitions();
		subscribeToAchievementNotifications();
	}
}

}