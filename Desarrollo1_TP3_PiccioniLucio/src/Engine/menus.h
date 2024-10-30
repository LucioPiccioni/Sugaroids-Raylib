#pragma once

#include "raylib.h"

enum class Menus
{
	MainMenu,
	Playing,
	Rules,
	Credits,
	WantToExit,
	None,

	Replay,

	Resume,

	ConfirmExit,
	CancelExit,
	Exit

};

extern const float buttonWidth;
extern const float buttonHeight;
extern const float buttonSpacing;

extern const float titlesFontSize;
extern const float textFontSize;
extern const float scoreFontSize;