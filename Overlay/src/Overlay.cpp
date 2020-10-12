#include "pch.h"
#include "Overlay.h"
#include "Loader.h"
#include "achievement_manager_ui.h"
#include <thread>
#include <future>

#define POPUP_DURATION_MS	3000

// Forward declaration, as suggested by imgui_impl_win32.cpp#L270
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Adapted from: https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook
namespace Overlay {

HRESULT(WINAPI* originalPresent) (IDXGISwapChain*, UINT, UINT);
HRESULT(WINAPI* originalResizeBuffers) (IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
LRESULT(WINAPI* originalWindowProc)(HWND, UINT, WPARAM, LPARAM);

HWND gWindow = nullptr;
ID3D11Device* gD3D11Device = nullptr;
ID3D11DeviceContext* gContext = nullptr;
ID3D11RenderTargetView* gRenderTargetView = nullptr;

bool bInit = false;
bool bShowInitPopup = true;
bool bShowAchievementManager = false;

Achievements* achievements = nullptr;
UnlockAchievementFunction* unlockAchievement = nullptr;

LRESULT WINAPI WindowProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if(uMsg == WM_KEYDOWN) {
		// Shift + F5 pressed?
		if(GetKeyState(VK_SHIFT) & 0x8000 && wParam == VK_F5) {
			bShowInitPopup = false; // Hide the popup
			bShowAchievementManager = !bShowAchievementManager; // Toggle the overlay
		}
	}

	if(bShowAchievementManager) {
		// Civilization VI mouse input fix
		switch(uMsg)
		{
		case WM_POINTERDOWN:
			uMsg = WM_LBUTTONDOWN;
			break;
		case WM_POINTERUP:
			uMsg = WM_LBUTTONUP;
			break;
		case WM_POINTERWHEEL:
			uMsg = WM_MOUSEWHEEL;
			break;
		case WM_POINTERUPDATE:
			uMsg = WM_SETCURSOR;
			break;
		}
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	} else {
		return CallWindowProc(originalWindowProc, hWnd, uMsg, wParam, lParam);
	}
}

HRESULT WINAPI hookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if(!bInit) {
		if(SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&gD3D11Device))) {
			gD3D11Device->GetImmediateContext(&gContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			gWindow = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
#pragma warning(suppress: 6387)
			gD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &gRenderTargetView);
			pBackBuffer->Release();
			originalWindowProc = (WNDPROC)SetWindowLongPtr(gWindow, GWLP_WNDPROC, (LONG_PTR)WindowProc);
			if(originalWindowProc == NULL){
				Logger::error("Failed to SetWindowLongPtr. Error code: %d", GetLastError());
				return originalPresent(pSwapChain, SyncInterval, Flags);
			}
			AchievementManagerUI::InitImGui(gWindow, gD3D11Device, gContext);
			bInit = true;
			Loader::AsyncLoadIcons();
		} else {
			return originalPresent(pSwapChain, SyncInterval, Flags);
		}
	}

	// Now that we are hooked, it's time to render the Achivement Manager

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if(bShowInitPopup)
		AchievementManagerUI::DrawInitPopup();

	if(bShowAchievementManager)
		AchievementManagerUI::DrawAchievementList();

	ImGui::Render();

	if(gRenderTargetView)
		gContext->OMSetRenderTargets(1, &gRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return originalPresent(pSwapChain, SyncInterval, Flags);
}

/**
 * We are hooking ResizeBuffer function in order to release allocated resources
 * and reset init flag, so that our overlay can reinitialize with new window size.
 * Without it, the game will crash on window resize.
 */
HRESULT WINAPI hookedResizeBuffer(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags){
	Logger::debug("hookedResizeBuffer");

	if(bInit){
		AchievementManagerUI::ShutdownImGui();

		// Restore original WndProc. Crashes without it.
		SetWindowLongPtr(gWindow, GWLP_WNDPROC, (LONG_PTR)originalWindowProc);

		// Release RTV according to: https://www.unknowncheats.me/forum/2638258-post8.html
		if(gRenderTargetView){
			gRenderTargetView->Release();
			gRenderTargetView = nullptr;
		}

		bInit = false;
	}
	return originalResizeBuffers(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

// Table with method indices: https://github.com/Rebzzel/kiero/blob/master/METHODSTABLE.txt
#define D3D11_Present		8
#define D3D11_ResizeBuffers	13

void Init(HMODULE hMod, Achievements* pAchievements, UnlockAchievementFunction* fnUnlockAchievement) {
	achievements = pAchievements;
	unlockAchievement = fnUnlockAchievement;

	// Init asynchronously to keep the main thread going
	static auto initJob = std::async(std::launch::async, []() {
#pragma warning(suppress: 26812)
		auto result = kiero::init(kiero::RenderType::D3D11);
		if(result != kiero::Status::Success){
			Logger::debug("Kiero: result code = %d", result);
			if(result == kiero::Status::ModuleNotFoundError)
				Logger::error("Failed to initialize kiero. Are you sure you are running a DirectX 11 game?");
			else
				Logger::error("Failed to initialize kiero. Error code: %d", result);

			return;
		}
		Logger::ovrly("Kiero: Successfully initialized");

		// Hook Present
		kiero::bind(D3D11_Present, (void**)&originalPresent, hookedPresent);
		Logger::ovrly("Kiero: Successfully hooked Present");

		// Hook ResizeBuffers
		kiero::bind(D3D11_ResizeBuffers, (void**)&originalResizeBuffers, hookedResizeBuffer);
		Logger::ovrly("Kiero: Successfully hooked ResizeBuffers");

		// Hide the popup after POPUP_DURATION_MS time
		static auto hidePopupJob = std::async(std::launch::async, [&]() {
			Sleep(POPUP_DURATION_MS);
			bShowInitPopup = false;
		});
	});
}

void Shutdown() {
	AchievementManagerUI::ShutdownImGui();
	SetWindowLongPtr(gWindow, GWLP_WNDPROC, (LONG_PTR)originalWindowProc);
	kiero::shutdown();
	Logger::ovrly("Kiero: Shutdown");
	// TODO: Clear the achievement vector as well?
}

}