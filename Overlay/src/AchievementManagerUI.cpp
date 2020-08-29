#include "pch.h"
#include "AchievementManagerUI.h"

namespace AchievementManagerUI{

Achievements* achievements;
UnlockAchievementFunction* unlockAchievement = nullptr;

ImFont* bigFont = nullptr;

void init(Achievements& achievements, UnlockAchievementFunction* unlockAchievement){
	AchievementManagerUI::achievements = &achievements;
	AchievementManagerUI::unlockAchievement = unlockAchievement;
}

void initImGui(void* pWindow, ID3D11Device* pD3D11Device, ID3D11DeviceContext* pContext){
	ImGui::CreateContext();

	// Hopefully every Windows PC should have calibri font installed
	auto fontPath = "C:\\Windows\\Fonts\\calibri.ttf";

	auto io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(fontPath, 16.0f);
	io.IniFilename = NULL;

	{// Init big font
		float pixedSize = 64;
		ImFontConfig config;
		config.SizePixels = pixedSize;
		config.OversampleH = config.OversampleV = 1;
		config.PixelSnapH = true;
		bigFont = io.Fonts->AddFontFromFileTTF(fontPath, pixedSize, &config);
	}

	ImGui_ImplWin32_Init(pWindow);
	ImGui_ImplDX11_Init(pD3D11Device, pContext);

	io.ImeWindowHandle = pWindow; // Do I need this?
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

void renderOverlay(){
	ImGuiIO& io = ImGui::GetIO();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Set window background to black
	ImGui::Begin("AchievementManagerUI", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleColor();

	ImGui::SetWindowPos(ImVec2());
	ImGui::SetWindowSize(ImVec2(400, io.DisplaySize.y));

	//FitTextToWindow;
	FitTextToWindow(ImVec4(0, 1, 0, 1), "ScreamAPI Achievement Manager");

	ImGui::BeginChild("AchievementList", ImVec2(0, 0), false);
	for(int i = 0; i < achievements->size(); i++){
		auto&& achievement = achievements->at(i);

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
			ImGui::SetColumnWidth(1, 225);
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
					ImGui::TextColored(ImVec4(00, .75, 00, 1), "Unlocked");
					break;
				case UnlockState::Unlocking:
					ImGui::TextColored(ImVec4(.75, .75, 0, 1), "Unlocking...");
					break;
				case UnlockState::Locked:
					if(ImGui::Button("Unlock"))
						unlockAchievement(&achievement);
					break;
			}
		}
		ImGui::Columns(1);

		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::End();
}


}

