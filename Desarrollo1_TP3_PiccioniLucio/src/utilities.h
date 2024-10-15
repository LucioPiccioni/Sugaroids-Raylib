#pragma once

#include "raylib.h"
#include <string>

namespace Tools
{
	bool CheckMouseButtonCollition(Vector2 mouse, Rectangle button);
	void DrawButton(Rectangle rect, const std::string& text, Color& color, Font& font);
}