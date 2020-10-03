#include "pch.h"
#include "achievement_manager_ui.h"
#include <Overlay.h>

namespace AchievementManagerUI{

ImFont* bigFont = nullptr;
constexpr auto mainFontSize = 21.0f;
// Hopefully every Windows PC should have Segoeui font installed
auto fontPath = "C:\\Windows\\Fonts\\Segoeui.ttf"; 

void InitImGui(void* pWindow, ID3D11Device* pD3D11Device, ID3D11DeviceContext* pContext){
	ImGui::CreateContext();

	static const ImWchar ranges[] = { 0x0020, 0xFFFF, 0 }; // All ranges
	auto& io = ImGui::GetIO(); // It's important to get reference (auto&) instead of copy (auto)
	io.IniFilename = NULL;
	io.Fonts->AddFontFromFileTTF(fontPath, mainFontSize, NULL, ranges);

	{// Init big font
		float pixelSize = 64;
		ImFontConfig config;
		config.SizePixels = pixelSize;
		config.OversampleH = config.OversampleV = 1;
		config.PixelSnapH = true;
		bigFont = io.Fonts->AddFontFromFileTTF(fontPath, pixelSize, &config, ranges);
	}

	ImGui_ImplWin32_Init(pWindow);
	ImGui_ImplDX11_Init(pD3D11Device, pContext);
	Logger::ovrly("ImGui: Successfully initialized");
}

void ShutdownImGui(){
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	Logger::ovrly("ImGui: Shutdown");
}

// Adapted from: https://github.com/ocornut/imgui/issues/1018#issuecomment-491875324
void FitTextToWindow(const ImVec4 color, const char* text){
	ImGui::PushFont(bigFont);
	ImVec2 sz = ImGui::CalcTextSize(text);
	ImGui::PopFont();
	float canvasWidth = ImGui::GetWindowContentRegionWidth();
	float origScale = bigFont->Scale;
	bigFont->Scale = canvasWidth / sz.x;
	ImGui::PushFont(bigFont);
	ImGui::TextColored(color, "%s", text);
	ImGui::PopFont();
	bigFont->Scale = origScale;
}

void DrawAchievementList(){
	ImGuiIO& io = ImGui::GetIO();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Set window background to black
	ImGui::Begin("AchievementManagerUI", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleColor();

	ImGui::SetWindowPos(ImVec2());
	ImGui::SetWindowSize(ImVec2(400, io.DisplaySize.y));

	FitTextToWindow(ImVec4(0, 1, 0, 1), "  ScreamAPI Achievement Manager  ");

	ImGui::BeginChild("AchievementList", ImVec2(0, 0), false);
	for(unsigned int i = 0; i < Overlay::achievements->size(); i++){
		auto&& achievement = Overlay::achievements->at(i);

		ImGui::PushID(i);

		ImGui::Columns(3, "Achievement", false);
		ImGui::Separator();
		{// Icon
			ImGui::SetColumnWidth(0, 70);
			ImGui::Spacing();
			ImGui::Image(achievement.IconTexture, ImVec2(50, 50));
			ImGui::Spacing();
			ImGui::NextColumn();
		}
		{// Title & description
			ImGui::SetColumnWidth(1, 210);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), achievement.UnlockedDisplayName);
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.75, .75, .75, 1));
			ImGui::TextWrapped(achievement.UnlockedDescription);
			ImGui::PopStyleColor();
			ImGui::NextColumn();
		}
		{// Button or status text
			std::string hidden = achievement.IsHidden ? "[Hidden]" : "";
			ImGui::TextColored(ImVec4(.75, .75, .75, 1), hidden.c_str());
			ImGui::Spacing();

			switch(achievement.UnlockState){
				case UnlockState::Unlocked:
					ImGui::TextColored(ImVec4(00, .75, 00, 1), "Unlocked   ");
					break;
				case UnlockState::Unlocking:
					ImGui::TextColored(ImVec4(.75, .75, 0, 1), "Unlocking...");
					break;
				case UnlockState::Locked:
					if(ImGui::Button(" Unlock "))
						Overlay::unlockAchievement(&achievement);
					break;
			}
		}
		ImGui::Columns(1);

		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::End();
}

void DrawInitPopup(){
	static auto grayCol = ImVec4(.75, .75, .75, 1);
	static auto greenCol = ImVec4(0, .75, 0, 1);

	ImGuiIO& io = ImGui::GetIO();// (void) io;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1)); // Set window background to black
	ImGui::PushStyleColor(ImGuiCol_Border, greenCol);
	ImGui::Begin("InitPopup", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::SetWindowPos(ImVec2());
	ImGui::SetWindowSize(ImVec2(400, 80));

	{
		static std::string firstPart = "ScreamAPI Achievement Manager is";
		static std::string secondPart = "online";
		ImGui::PushFont(bigFont);
		ImVec2 sz = ImGui::CalcTextSize((firstPart + " " + secondPart).c_str());
		ImGui::PopFont();
		float canvasWidth = ImGui::GetWindowContentRegionWidth();
		float origScale = bigFont->Scale;
		bigFont->Scale = canvasWidth / sz.x;

		ImGui::Spacing();

		ImGui::PushFont(bigFont);
		ImGui::TextColored(grayCol, firstPart.c_str());
		ImGui::SameLine();
		ImGui::TextColored(greenCol, secondPart.c_str());
		ImGui::PopFont();
		bigFont->Scale = origScale;

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextColored(grayCol, "Press");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 1, 1), "[Shift] + [F5]");
		ImGui::SameLine();
		ImGui::TextColored(grayCol, "to open panel");
	}

	ImGui::End();
}

}

