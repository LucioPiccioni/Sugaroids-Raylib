#pragma once

#include "raylib.h"
#include <string>

namespace Tools
{
	bool CheckMouseButtonCollition(Vector2 mouse, Rectangle button);
	void DrawButton(Rectangle rect, const std::string& text, Color& color, Color& outline, Font& font);
}

extern int screenWidth;
extern int screenHeight;