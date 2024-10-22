#pragma once

#include "raylib.h"
#include <string>

namespace Tools
{
	bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
	bool CheckMouseButtonCollition(Vector2 mouse, Rectangle button);
	bool CheckIfOutOfBounds(Vector2& position, float& radius);
	void DrawButton(Rectangle rect, const std::string& text, Color& color, Color& outline, Font& font);
}

extern int screenWidth;
extern int screenHeight;
extern float timmerToCleanBuffer;