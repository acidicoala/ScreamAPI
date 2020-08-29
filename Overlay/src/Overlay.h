#pragma once
#include "pch.h"
#include "Overlay_types.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

namespace Overlay{

extern ID3D11Device* pD3D11Device;

void init(HMODULE hMod, Achievements& achievements, UnlockAchievementFunction* unlockAchievement);
void shutdown();

}
