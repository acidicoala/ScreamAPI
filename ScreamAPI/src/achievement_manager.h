#pragma once
#include "pch.h"
#include "Overlay_types.h"

namespace AchievementManager{

void findAchievement(const char* achievementID, std::function<void(Overlay_Achievement&)> callback);
void queryAchievementDefinitions();
void unlockAchievement(Overlay_Achievement* achievement);

}