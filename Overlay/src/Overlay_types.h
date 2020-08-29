#pragma once
#include "pch.h"

enum class UnlockState{
	Locked,
	Unlocked,
	Unlocking
};

struct Overlay_Achievement{
	const char* AchievementId;
	bool IsHidden;
	UnlockState UnlockState;
	const char* UnlockedDescription;
	const char* UnlockedDisplayName;
	const char* UnlockedIconURL;
	ID3D11ShaderResourceView* IconTexture;
};

typedef std::vector<Overlay_Achievement> Achievements;

typedef void (UnlockAchievementFunction)(Overlay_Achievement*);
