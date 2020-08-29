#pragma once

#include "Overlay_types.h"

namespace AchievementManagerUI{

void init(Achievements& achievements, UnlockAchievementFunction* unlockAchievement);
void initImGui(void* pWindow, ID3D11Device* pD3D11Device, ID3D11DeviceContext* pContext);
void renderOverlay();

}