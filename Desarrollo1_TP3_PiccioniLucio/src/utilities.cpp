#include "utilities.h"
#include "menus.h"

int screenWidth = 1024;
int screenHeight = 768;
float timmerToCleanBuffer = 0;

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

void Tools::DrawButton(Rectangle rect, const std::string& text, Color& color, Color& outline, Font& font)
{
	DrawRectangle(rect.x,rect.y,rect.width,rect.height, color);
	DrawRectangleLines(rect.x,rect.y,rect.width,rect.height, outline);
	Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(textFontSize), 1);
	Vector2 textPosition = { rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2 };
	DrawTextEx(font, text.c_str(), Vector2{ textPosition.x, textPosition.y }, textFontSize, 0, BLACK);
}