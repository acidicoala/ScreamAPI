#pragma once

#include "Overlay_types.h"

namespace AchievementManagerUI{

void Init(Achievements& achievements, UnlockAchievementFunction* unlockAchievement);
void InitImGui(void* pWindow, ID3D11Device* pD3D11Device, ID3D11DeviceContext* pContext);
void ShutdownImGui();
void DrawAchievementList();
void DrawInitPopup();

}
