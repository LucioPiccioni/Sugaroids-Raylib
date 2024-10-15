#include "utilities.h"
#include "menus.h"

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

void Tools::DrawButton(Rectangle rect, const std::string& text, Color color)
{
	DrawRectangleRec(rect, color);
	Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(textFontSize), 1);
	Vector2 textPosition = { rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2 };
	DrawText(text.c_str(), static_cast<int>(textPosition.x), static_cast<int>(textPosition.y), textFontSize, BLACK);
}