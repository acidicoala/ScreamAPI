#pragma once
#include "pch.h"
#include "ScreamAPI.h"

namespace Util{
template <typename T>
static bool vectorContains(std::vector<T> vector, T element){
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

struct QueryPlayerAchievementsContainer{
	void* originalClientData;
	EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback originalCompletionDelegate;
};

std::filesystem::path getDLLparentDir(HMODULE hModule);
void printAchievementDefinition(EOS_Achievements_DefinitionV2* definition);
void printPlayerAchievement(EOS_Achievements_PlayerAchievement* achievement);
void EOS_CALL queryDefinitionsComplete(const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data);
void unlockAchievement(Overlay_Achievement* achievement);
}