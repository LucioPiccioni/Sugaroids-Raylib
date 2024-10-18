#pragma once
#include "raylib.h"

enum class Menus
{
	MainMenu,
	Playing,
	Rules,
	Credits,
	Exit,
	None,

	Replay
};

extern const int buttonWidth;
extern const int buttonHeight;
extern const int buttonSpacing;

extern const int titlesFonstSize;
extern const int textFontSize;
extern const int scoreFontSize;