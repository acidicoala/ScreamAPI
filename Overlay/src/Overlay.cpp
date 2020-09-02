#include "pch.h"
#include "Overlay.h"
#include "Loader.h"
#include "achievement_manager_ui.h"
#include <thread>
#include <future>

// Forward declaration, as suggested by imgui_impl_win32.cpp#L270
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Adapted from: https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook
namespace Overlay {

#define POPUP_DURATION_MS	3000

HRESULT(__stdcall* originalPresent) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
WNDPROC originalWndProc = nullptr;
ID3D11Device* pD3D11Device = nullptr;

bool showAchievementManager = false;
bool showInitPopup = true;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if(uMsg == WM_KEYUP) {
		// Shift + F5 pressed?
		if(GetKeyState(VK_SHIFT) & 0x8000 && wParam == VK_F5) {
			showInitPopup = false; // Hide the popup
			showAchievementManager = !showAchievementManager; // Toggle the overlay
		}
	}

	if(showAchievementManager) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	} else {
		return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
	}
}

HRESULT __stdcall hookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	static ID3D11RenderTargetView* mainRenderTargetView = nullptr;
	static ID3D11DeviceContext* pContext = nullptr;
	static HWND pWindow = nullptr;

	static bool init = false;
	if(!init) {
		if(SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**) &pD3D11Device))) {
			pD3D11Device->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			pWindow = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer);
#pragma warning(suppress: 6387)
			pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			originalWndProc = (WNDPROC) SetWindowLongPtr(pWindow, GWLP_WNDPROC, (LONG_PTR) WndProc);
			AchievementManagerUI::initImGui(pWindow, pD3D11Device, pContext);
			init = true;
		} else {
			return originalPresent(pSwapChain, SyncInterval, Flags);
		}
	}

	// Now that we are hooked, it's time to render the Achivement Manager

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if(showInitPopup)
		AchievementManagerUI::drawInitPopup();

	if(showAchievementManager)
		AchievementManagerUI::drawAchievementList();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return originalPresent(pSwapChain, SyncInterval, Flags);
}

void initThread(LPVOID lpReserved) {
	while(kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success);
	Logger::ovrly("Kiero: Successfully initialized");

	kiero::bind(8, (void**) &originalPresent, hookedPresent);
	Logger::ovrly("Kiero: Successfully binded");

	// Hide the popup after POPUP_DURATION_MS time
	static auto hidePopupJob = std::async(std::launch::async, [&]() {
		Sleep(POPUP_DURATION_MS);
		showInitPopup = false;
	});
}

void Overlay::init(HMODULE hMod, Achievements& achievements, UnlockAchievementFunction* unlockAchievement) {
	AchievementManagerUI::init(achievements, unlockAchievement);
	std::thread(initThread, hMod).detach();
}

void Overlay::shutdown() {
	AchievementManagerUI::shutdownImGui();
	kiero::shutdown();
	Logger::ovrly("Kiero: Shutdown");
	// TODO: Clear the achievement vector as well?
}

}