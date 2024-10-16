#include "scene.h"

void Scene::DrawGamePlay(std::vector <Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		DrawTexturePro(bulletsImage, Rectangle{ 0, 0, (float)bulletsImage.width, (float)bulletsImage.height },  // Fuente (imagen completa)
			Rectangle{ bullets[i].position.x, bullets[i].position.y,  bullets[i].size,  bullets[i].size },  // Destino (posicion y tamaño)
			Vector2{ bullets[i].size / 2, bullets[i].size / 2 },  // Offset de origen para el centro
			0.0f,
			WHITE);
	}


	DrawTexturePro(
		playerImage,  // La textura original
		Rectangle{ 0, 0, (float)playerImage.width, (float)playerImage.height },  // Fuente: toda la imagen original
		Rectangle{ player.pos.x, player.pos.y, 64, 64 },  // Destino: posición y nuevo tamaño 64x64
		Vector2{ 32, 32 },  // Offset (centro) si es necesario, depende de cómo quieras alinear el sprite
		player.angle,  // Rotación (0 si no quieres rotar)
		WHITE  // Color (generalmente WHITE para no aplicar ningún tinte)
	);


	for (int i = 0; i < sugaroids.size(); i++) {
		DrawTexturePro(
			sugaroidImage,
			Rectangle{ 0, 0, (float)sugaroidImage.width, (float)sugaroidImage.height },  // Fuente (imagen completa)
			Rectangle{ sugaroids[i].position.x, sugaroids[i].position.y, sugaroids[i].size, sugaroids[i].size },  // Destino (posición y tamaño)
			Vector2{ sugaroids[i].size / 2, sugaroids[i].size / 2 },  // Offset del centro
			0.0f,  // Rotación (puedes agregar rotación si es necesario)
			WHITE);
	}
}

void Scene::DrawMainMenu(Menus& gameState, Font& font, Texture2D& gamesTitle, int& screenWidth, int& screenHeight)
{
	const int maxButtons = 5;

	Vector2 mouse;
	Button button[maxButtons] = {};

	int startX, startY;

	startX = (screenWidth - buttonWidth) / 2;
	startY = (screenHeight - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1)));

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { static_cast<float>(startX), static_cast<float>(startY + i * (buttonHeight + buttonSpacing)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
	}

	button[0].option = Menus::Playing;
	button[1].option = Menus::Rules;
	button[2].option = Menus::Credits;
	button[3].option = Menus::Exit;

	Color outline = BLACK;

	mouse = GetMousePosition();

	for (int i = 0; i < maxButtons; i++)
	{
		if (Tools::CheckMouseButtonCollition(mouse, button[i].rec))
		{
			button[i].color = WHITE;

			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				button[i].color = YELLOW;
			}

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
				gameState = button[i].option;
		}
	}

	float scaleFactor = std::min((float)screenWidth / gamesTitle.width, (float)screenHeight / gamesTitle.height);

	float newWidth = gamesTitle.width * scaleFactor;
	float newHeight = gamesTitle.height * scaleFactor;

	DrawTexturePro(
		gamesTitle,
		Rectangle{ 0, 0, (float)gamesTitle.width, (float)gamesTitle.height },  // Fuente: toda la imagen original
		Rectangle{ (screenWidth - newWidth) / 2, (screenHeight / 2 - newHeight), newWidth, newHeight },  // Destino: posición centrada con tamaño escalado
		Vector2{ 0, 0 },  // Offset de origen (esquina superior izquierda)
		0.0f,  // Sin rotación
		WHITE  // Sin tintes de color
	);

	for (int i = 0; i < maxButtons; i++)
	{
		switch (button[i].option)
		{
		case Menus::Playing:

			Tools::DrawButton(button[i].rec, "Play", button[i].color, outline, font);
			break;

		case Menus::Rules:

			Tools::DrawButton(button[i].rec, "Rules", button[i].color, outline, font);
			break;

		case Menus::Credits:

			Tools::DrawButton(button[i].rec, "Credits", button[i].color, outline, font);
			break;

		case Menus::Exit:

			Tools::DrawButton(button[i].rec, "Exit", button[i].color, outline, font);
			break;

		default: break;
		}
	}

}

void Scene::DrawCredits(int screenWidth, int screenHeight, Font customFont)
{
	const int titleFontSize = 40;
	const int textFontSize = 20;
	const int smallFontSize = 15;

	Vector2 titlePos = { screenWidth / 2 - MeasureTextEx(customFont, "Credits", titleFontSize, 2).x / 2, screenHeight / 4 };
	Vector2 developerPos = { screenWidth / 2 - MeasureTextEx(customFont, "Developer: Lucio Stefano Piccioni.", textFontSize, 2).x / 2, screenHeight / 4 + 60 };
	Vector2 musicPos = { screenWidth / 2 - MeasureTextEx(customFont, "Music:", textFontSize, 2).x / 2, screenHeight / 2 - 60 };
	Vector2 music1Pos = { screenWidth / 2 - MeasureTextEx(customFont, "1. 'Game Over!' - Harris Cole", smallFontSize, 2).x / 2, screenHeight / 2 - 30 };
	Vector2 music2Pos = { screenWidth / 2 - MeasureTextEx(customFont, "2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", smallFontSize, 2).x / 2, screenHeight / 2 - 10 };
	Vector2 music3Pos = { screenWidth / 2 - MeasureTextEx(customFont, "3. 'Quiet Nights' - JEN", smallFontSize, 2).x / 2, screenHeight / 2 + 10 };
	Vector2 music4Pos = { screenWidth / 2 - MeasureTextEx(customFont, "4. 'Facade' - JEN", smallFontSize, 2).x / 2, screenHeight / 2 + 30 };
	Vector2 soundEffectsPos = { screenWidth / 2 - MeasureTextEx(customFont, "Sound Effects:", textFontSize, 2).x / 2, screenHeight / 2 + 60 };
	Vector2 chipTonePos = { screenWidth / 2 - MeasureTextEx(customFont, "ChipTone", smallFontSize, 2).x / 2, screenHeight / 2 + 90 };
	Vector2 menuPos = { screenWidth / 2 - MeasureTextEx(customFont, "Press ESC to go back to the Menu.", smallFontSize, 2).x / 2, screenHeight - 40 };

	DrawTextEx(customFont, "Credits", titlePos, titleFontSize, 2, BLACK);
	DrawTextEx(customFont, "Developer: Lucio Stefano Piccioni.", developerPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Music:", musicPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "1. 'Game Over!' - Harris Cole", music1Pos, smallFontSize, 2, BLACK);
	DrawTextEx(customFont, "2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", music2Pos, smallFontSize, 2, BLACK);
	DrawTextEx(customFont, "3. 'Quiet Nights' - JEN", music3Pos, smallFontSize, 2, BLACK);
	DrawTextEx(customFont, "4. 'Facade' - JEN", music4Pos, smallFontSize, 2, BLACK);
	DrawTextEx(customFont, "Sound Effects:", soundEffectsPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "ChipTone", chipTonePos, smallFontSize, 2, BLACK);
	DrawTextEx(customFont, "Press ESC to go back to the Menu.", menuPos, smallFontSize, 2, BLACK);
}

void Scene::DrawGameRules(int screenWidth, int screenHeight, Font customFont)
{
	ClearBackground(RAYWHITE);

	int titleFontSize = 40;
	int textFontSize = 20;
	int lineSpacing = 30;
	int sectionSpacing = 50;

	Vector2 titlePos = { screenWidth / 2 - MeasureTextEx(customFont, "Game Rules", titleFontSize, 2).x / 2, screenHeight / 10 };

	Vector2 controlsTitlePos = { screenWidth / 2 - MeasureTextEx(customFont, "Controls:", textFontSize, 2).x / 2, titlePos.y + titleFontSize + sectionSpacing };
	Vector2 upArrowPos = { screenWidth / 2 - MeasureTextEx(customFont, "Up Arrow: Move Up", textFontSize, 2).x / 2, controlsTitlePos.y + lineSpacing };
	Vector2 downArrowPos = { screenWidth / 2 - MeasureTextEx(customFont, "Down Arrow: Move Down", textFontSize, 2).x / 2, upArrowPos.y + lineSpacing };
	Vector2 leftArrowPos = { screenWidth / 2 - MeasureTextEx(customFont, "Left Arrow: Move Left", textFontSize, 2).x / 2, downArrowPos.y + lineSpacing };
	Vector2 rightArrowPos = { screenWidth / 2 - MeasureTextEx(customFont, "Right Arrow: Move Right", textFontSize, 2).x / 2, leftArrowPos.y + lineSpacing };
	Vector2 firePos = { screenWidth / 2 - MeasureTextEx(customFont, "Left Click: Fire", textFontSize, 2).x / 2, rightArrowPos.y + lineSpacing };
	Vector2 escPausePos = { screenWidth / 2 - MeasureTextEx(customFont, "ESC: Pause", textFontSize, 2).x / 2, firePos.y + lineSpacing };

	Vector2 points1Pos = { screenWidth / 2 - MeasureTextEx(customFont, "Points for destroying sugaroids: 25", textFontSize, 2).x / 2, escPausePos.y + sectionSpacing };
	Vector2 points2Pos = { screenWidth / 2 - MeasureTextEx(customFont, "Points for avoiding sugaroids: 5", textFontSize, 2).x / 2, points1Pos.y + lineSpacing };
	Vector2 lives1Pos = { screenWidth / 2 - MeasureTextEx(customFont, "You start with 3 lives.", textFontSize, 2).x / 2, points2Pos.y + lineSpacing };
	Vector2 lives2Pos = { screenWidth / 2 - MeasureTextEx(customFont, "Lose all lives to end the game.", textFontSize, 2).x / 2, lives1Pos.y + lineSpacing };

	Vector2 backToMenuPos = { screenWidth / 2 - MeasureTextEx(customFont, "Press ESC to return to menu", textFontSize, 2).x / 2, screenHeight - 60 };

	DrawTextEx(customFont, "Game Rules", titlePos, titleFontSize, 2, BLACK);
	DrawTextEx(customFont, "Controls:", controlsTitlePos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Up Arrow: Move Up", upArrowPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Down Arrow: Move Down", downArrowPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Left Arrow: Move Left", leftArrowPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Right Arrow: Move Right", rightArrowPos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Left Click: Fire", firePos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "ESC: Pause", escPausePos, textFontSize, 2, BLACK);

	DrawTextEx(customFont, "Points for destroying sugaroids: 25", points1Pos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Points for avoiding sugaroids: 5", points2Pos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "You start with 3 lives.", lives1Pos, textFontSize, 2, BLACK);
	DrawTextEx(customFont, "Lose all lives to end the game.", lives2Pos, textFontSize, 2, BLACK);

	DrawTextEx(customFont, "Press ESC to return to menu", backToMenuPos, textFontSize, 2, BLACK);
}

void Scene::DrawGameOver(Menus& gameState, Font& font, int& screenWidth, int& screenHeight)
{
	const int maxButtons = 3;
	Vector2 mouse = GetMousePosition();
	Button button[maxButtons] = {};

	Color outline = BLACK;

	int startX, startY;

	startX = (screenWidth - buttonWidth) / 2;
	startY = ((screenHeight - screenHeight / 5) - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1)));

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { static_cast<float>(startX), static_cast<float>(startY + i * (buttonHeight + buttonSpacing)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
	}

	button[0].option = Menus::Replay;
	button[1].option = Menus::MainMenu;
	button[2].option = Menus::Exit;

	// Procesar el estado del mouse y los clics
	for (int i = 0; i < maxButtons; i++)
	{
		if (Tools::CheckMouseButtonCollition(mouse, button[i].rec))
		{
			button[i].color = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? YELLOW : WHITE;


			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
				gameState = button[i].option;
		}
	}

	Vector2 gameOverTextSize = MeasureTextEx(font, "GAME OVER", 50, 0);
	Vector2 gameOverPos = { static_cast<float>(screenWidth) / 2 - gameOverTextSize.x / 2, static_cast<float>(screenHeight) / 3 };

	DrawTextEx(font, "GAME OVER", gameOverPos, 50, 0, RED);

	for (int i = 0; i < maxButtons; i++)
	{
		switch (button[i].option)
		{
		case Menus::Replay:
			Tools::DrawButton(button[i].rec, "Replay", button[i].color, outline, font);
			break;
		case Menus::MainMenu:
			Tools::DrawButton(button[i].rec, "MainMenu", button[i].color, outline, font);
			break;
		case Menus::Exit:
			Tools::DrawButton(button[i].rec, "Exit", button[i].color, outline, font);
			break;
		}
	}
}