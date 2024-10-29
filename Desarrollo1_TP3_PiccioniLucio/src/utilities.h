#pragma once

#include <string>

#include "raylib.h"

namespace Tools
{
	bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
	bool CheckMouseButtonCollition(Vector2 mouse, Rectangle button);
	bool CheckIfOutOfBounds(Vector2 position, float radius, int newScreenWidth, int newScreenHeight);
	void DrawButton(Rectangle rect, const std::string text, Color color, Color outline, Font font, int newScreenWidth, int newScreenHeight, float scaleFactor);

	void AdjustSizeAndPos(Vector2& position, float& size, float& radius, int newScreenWidth, int newScreenHeight);
}

extern int screenWidth;
extern int screenHeight;

extern float timmerToCleanBuffer;