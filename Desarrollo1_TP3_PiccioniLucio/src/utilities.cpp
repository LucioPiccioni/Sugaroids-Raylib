#include "utilities.h"
#include "menus.h"

int screenWidth = 1024;
int screenHeight = 768;

float timmerToCleanBuffer = 0;

void Tools::OpenURL(const std::string& url) { std::string PESADO = url; }

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
		mouse.y <= button.y + button.height)
	{   // above the bottom

		return true;
	}

	return false;
}

bool Tools::CheckIfOutOfBounds(Vector2 position, float radius, int newScreenWidth, int newScreenHeight)
{
	return  (position.x + radius < 0 || position.x - radius > newScreenWidth || position.y + radius < 0 || position.y - radius > newScreenHeight);
}

void Tools::DrawButton(Rectangle rect, const std::string text, Color color, Color outline, Font font, int newScreenWidth, int newScreenHeight, float scaleFactor)
{
    float scaleX = static_cast<float>(newScreenWidth) / static_cast<float>(screenWidth);
    float scaleY = static_cast<float>(newScreenHeight) / static_cast<float>(screenHeight);

	float centerX = rect.x + rect.width / 2;
	float centerY = rect.y + rect.height / 2;

	rect.width *= scaleX;
	rect.height *= scaleY;

	rect.x = centerX - rect.width / 2;
	rect.y = centerY - rect.height / 2;

    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, 2, outline);

    Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(textFontSize) * scaleFactor, 1);

    Vector2 textPosition = {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + (rect.height - textSize.y) / 2
    };

	DrawTextPro(font, 
		text.c_str(), 
		textPosition, 
		Vector2{0,0},
		0,
		textFontSize * scaleFactor, 
		0, 
		BLACK);
}



void Tools::AdjustSizeAndPos(Vector2& position, float& size, float& radius,int newScreenWidth, int newScreenHeight)
{
	float scaleX = static_cast<float>(newScreenWidth) / static_cast<float>(screenWidth);
	float scaleY = static_cast<float>(newScreenHeight) / static_cast<float>(screenHeight);

	position.x *= scaleX;
	position.y *= scaleY;

	size =
	
	radius = radius / 2;
}

