#include "utilities.h"
#include "menus.h"

float timmerToCleanBuffer = 0;

bool Tools::CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
{
	float distance = sqrtf(powf(center2.x - center1.x, 2) + powf(center2.y - center1.y, 2));

	return distance <= (radius1 + radius2);
}

bool Tools::CheckMouseButtonCollition(Vector2 mouse, Rectangle button)
{
	// is the point inside the rectangle's bounds?
	if (mouse.x >= button.x &&        // right of the left edge AND
		mouse.x <= button.x + button.width &&   // left of the right edge AND
		mouse.y >= button.y &&        // below the top AND
		mouse.y <= button.y + button.height) {   // above the bottom

		return true;
	}

	return false;
}

bool Tools::CheckIfOutOfBounds(Vector2 position, float radius, int screenWidth, int screenHeight)
{
	return  (position.x + radius < 0 || position.x - radius > screenWidth || position.y + radius < 0 || position.y - radius > screenHeight);
}



void Tools::DrawButton(Rectangle rect, const std::string text, Color color, Color outline, Font font)
{
	DrawRectangleRec(rect, color);
	DrawRectangleLinesEx(rect, 2, outline);
	Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(textFontSize), 1);
	Vector2 textPosition = { rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2 };
	DrawTextEx(font, text.c_str(), Vector2{ textPosition.x, textPosition.y }, textFontSize, 0, BLACK);
}
