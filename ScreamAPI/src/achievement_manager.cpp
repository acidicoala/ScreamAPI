#include "pch.h"
#include "achievement_manager.h"
#include "ScreamAPI.h"
#include "util.h"
#include "Loader.h"
#include "eos-sdk\eos_achievements.h"
#include <Overlay.h>
#include <future>

using namespace Util;

namespace AchievementManager{

Achievements achievements;

/**
 * A small utility function that copies the c string into a newly allocated memory
 * @return Pointer to the new string
 */
char* copy_c_string(const char* c_string){
	// Get string size
	auto string_size = strlen(c_string) + 1;// +1 for null terminator

	// Allocate enough memory for the new string
	char* new_string = new char[string_size];

	// Copy the string contents
	strcpy_s(new_string, string_size, c_string);

	return new_string;
}

void printAchievementDefinition(EOS_Achievements_DefinitionV2* definition){
	if(definition == nullptr){
		Logger::ach("Invalid Achievement Definition");
	} else{
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
		// Escape whole string because of % signs in URLs
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
		// Escape whole string because of % signs in URLs
		Logger::ach("%s", ss.str().c_str());
	}
}

/**
 * A callback function that is executed when EOS has done unlocking the achievement at our request.
 * @see EOS_Achievements_UnlockAchievements
 */
void EOS_CALL unlockAchievementsComplete(const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data){
	auto achievement = (Overlay_Achievement*) Data->ClientData;

	// Abort if something went wrong
	if(Data->ResultCode != EOS_EResult::EOS_Success){
		achievement->UnlockState = UnlockState::Locked;
		Logger::error("Failed to unlock the achievement: %s. Error string: %s",
					  achievement->AchievementId,
					  EOS_EResult_ToString(Data->ResultCode));
		return;
	}

	// Otherwise, update the achievement state
	achievement->UnlockState = UnlockState::Unlocked;
	Logger::info("Successfully unlocked the achievement: %s", achievement->AchievementId);
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

	EOS_Achievements_UnlockAchievements(getHAchievements(), &Options, achievement, unlockAchievementsComplete);
}

void EOS_CALL queryPlayerAchievementsComplete(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data){
	// Abort if something went wrong
	if(Data->ResultCode != EOS_EResult::EOS_Success){
		Logger::error("Failed to query player achievements. Result string: %s",
					  EOS_EResult_ToString(Data->ResultCode));
		return;
	}

	// Get number of player achievements
	static EOS_Achievements_GetPlayerAchievementCountOptions GetCountOptions{
		EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST,
		Util::getProductUserId()
	};
	auto playerAchievementsCount = EOS_Achievements_GetPlayerAchievementCount(Util::getHAchievements(),
																			  &GetCountOptions);
	// Iterate over queried player achievements and update our own structs
	for(unsigned int i = 0; i < playerAchievementsCount; i++){
		EOS_Achievements_CopyPlayerAchievementByIndexOptions CopyAchievementOptions{
			EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST,
			Util::getProductUserId(),
			i
		};
		EOS_Achievements_PlayerAchievement* OutAchievement;
		EOS_Achievements_CopyPlayerAchievementByIndex(Util::getHAchievements(),
													  &CopyAchievementOptions,
													  &OutAchievement);
		printPlayerAchievement(OutAchievement);

		// Update our achievement array if this achievement is unlocked
		if(OutAchievement->UnlockTime != -1){
			// Find the corresponding achievement in our achievement array
			for(auto& achievement : achievements){
				// Compare by AchievementId
				if(!strcmp(achievement.AchievementId, OutAchievement->AchievementId)){
					// Found it
					achievement.UnlockState = UnlockState::Unlocked;
					break;
				}
			}
		}

		EOS_Achievements_PlayerAchievement_Release(OutAchievement);
	}

	// Initialize Overlay
	Overlay::init(ScreamAPI::thisDLL, achievements, unlockAchievement);

}

/**
 * A callback function that is executed when EOS has done querying achievement definitions at out request.
 *
 * @see queryAchievements
 * @see EOS_Achievements_QueryDefinitions
 */
void EOS_CALL queryDefinitionsComplete(const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data){
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
	auto achievementDefinitionCount = EOS_Achievements_GetAchievementDefinitionCount(Util::getHAchievements(),
																					 &GetCountOptions);
	// Iterate over queried achievement definitions and save them to our own structs
	for(uint32_t i = 0; i < achievementDefinitionCount; i++){
		EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions DefinitionOptions{
			EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYINDEX_API_LATEST,
			i
		};
		EOS_Achievements_DefinitionV2* OutDefinition;
		EOS_Achievements_CopyAchievementDefinitionV2ByIndex(Util::getHAchievements(),
															&DefinitionOptions,
															&OutDefinition);
		printAchievementDefinition(OutDefinition);

		achievements.push_back(
			{
				copy_c_string(OutDefinition->AchievementId),
				(bool) OutDefinition->bIsHidden,
				UnlockState::Locked,
				copy_c_string(OutDefinition->UnlockedDescription),
				copy_c_string(OutDefinition->UnlockedDisplayName),
				copy_c_string(OutDefinition->UnlockedIconURL),
				nullptr
			});

		EOS_Achievements_DefinitionV2_Release(OutDefinition);
	}

	// Query player achievements to update the ones that are already unlocked
	EOS_Achievements_QueryPlayerAchievementsOptions QueryAchievementsOptions = {
			EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST,
			Util::getProductUserId()
	};
	EOS_Achievements_QueryPlayerAchievements(Util::getHAchievements(),
											 &QueryAchievementsOptions,
											 nullptr,
											 queryPlayerAchievementsComplete);

	// Download the files and load the textures in a background thread to keep UI responsive
	Overlay::init(ScreamAPI::thisDLL, achievements, unlockAchievement);
	Loader::asyncLoadIcons(achievements);
}

void queryAchievementDefinitions(){
	// Query achievement definitions
	EOS_Achievements_QueryDefinitionsOptions QueryDefinitionsOptions = {
			EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST,
			getProductUserId(),
			getEpicAccountId(),
			nullptr,
			0
	};
	EOS_Achievements_QueryDefinitions(Util::getHAchievements(),
									  &QueryDefinitionsOptions,
									  nullptr,
									  queryDefinitionsComplete);
}

}