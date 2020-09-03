#pragma once
#include "pch.h"
#include "Overlay_types.h"


namespace Overlay{

extern ID3D11Device* gD3D11Device;

void init(HMODULE hMod, Achievements& achievements, UnlockAchievementFunction* unlockAchievement);
void shutdown();

}
