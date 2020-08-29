#include "pch.h"
#include "util.h"
#include "eos-sdk\eos_achievements.h"
#include <Loader.h>
#include <thread>

namespace Util{

std::filesystem::path getDLLparentDir(HMODULE hModule){
	WCHAR modulePathBuffer[MAX_PATH];
	GetModuleFileName(hModule, modulePathBuffer, MAX_PATH);

	std::filesystem::path modulePath = modulePathBuffer;
	return modulePath.parent_path();
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
 * A small utility function that copies the c string into a newly allocated memory
 * @return Pointer to the new string
 */
char* copy_c_string(const char* c_str){
	// Get string size
	auto string_size = strlen(c_str) + 1;// +1 for null terminator

	// Allocate enough memory for the new string
	char* new_string = new char[string_size];

	// Copy the string contents
	strcpy_s(new_string, string_size, c_str);

	return new_string;
}

/**
 * A callback function that is executed when EOS has done querying achievement definitions.
 * @see EOS_Achievements_QueryDefinitions
 */
void EOS_CALL queryDefinitionsComplete(const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data){
	// Abort if something went wrong
	Logger::debug("ScreamAPI EOS_Achievements_QueryDefinitions result: %s", EOS_EResult_ToString(Data->ResultCode));
	if(Data->ResultCode != EOS_EResult::EOS_Success){
		Logger::error("Failed to query achievement definitions");
		return;
	}

	// Get the achievements handle, if was not initialized yet
	if(!ScreamAPI::hAchievements)
		EOS_Platform_GetAchievementsInterface(ScreamAPI::hPlatform);

	// Get number of all achievements in game
	static EOS_Achievements_GetAchievementDefinitionCountOptions QueryOptions{
		EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST
	};
	auto achievementDefinitionCount = EOS_Achievements_GetAchievementDefinitionCount(ScreamAPI::hAchievements, &QueryOptions);

	for(uint32_t i = 0; i < achievementDefinitionCount; i++){
		EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions DefinitionOptions{
			EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYINDEX_API_LATEST,
			i
		};
		EOS_Achievements_DefinitionV2* OutDefinition;
		EOS_Achievements_CopyAchievementDefinitionV2ByIndex(ScreamAPI::hAchievements,
															&DefinitionOptions,
															&OutDefinition);
		ScreamAPI::achievements.push_back(
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

	// Download the files and load the textures in a background thread to keep UI responsive
	Loader::asyncLoadIcons(ScreamAPI::achievements);
	//std::thread(Loader::asyncLoadIcons, std::ref(ScreamAPI::achievements)).detach();

	auto container = reinterpret_cast<QueryPlayerAchievementsContainer*>(Data->ClientData);

	EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo ResponseData{
		EOS_EResult::EOS_Success,
		container->originalClientData,
		ScreamAPI::ProductUserId
	};

	container->originalCompletionDelegate(&ResponseData);
	delete container;
}

/**
 * A callback function that is executed when EOS has done unlocking the achievement at our request.
 * @see EOS_Achievements_UnlockAchievements
 */
void EOS_CALL unlockAchievementsComplete(const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data){
	auto achievement = (Overlay_Achievement*) Data->ClientData;

	// Abort if something went wrong
	if(Data->ResultCode != EOS_EResult::EOS_Success){
		Logger::error("Failed to unlock the achievement: %s", achievement->AchievementId);
		achievement->UnlockState = UnlockState::Locked;
		return;
	}

	// Otherwise, update the achievement state
	Logger::info("Successfully unlocked the achievement: %s", achievement->AchievementId);
	achievement->UnlockState = UnlockState::Unlocked;
}

/**
 * A callback function that is executed when an Unlock button is pressed in the Overlay
 */
void unlockAchievement(Overlay_Achievement* achievement){
	achievement->UnlockState = UnlockState::Unlocking;

	// Get the ProductUserId, if was not initialized yet
	if(!ScreamAPI::ProductUserId){
		auto hConnect = EOS_Platform_GetConnectInterface(ScreamAPI::hPlatform);
		EOS_Connect_GetLoggedInUserByIndex(hConnect, 0);
	}

	EOS_Achievements_UnlockAchievementsOptions Options = {
		EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST,
		ScreamAPI::ProductUserId,
		const_cast<const char**>(&achievement->AchievementId),
		1
	};

	EOS_Achievements_UnlockAchievements(ScreamAPI::hAchievements, &Options, achievement, unlockAchievementsComplete);
}

}
