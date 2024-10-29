#include "scene.h"

#include "button.h"
#include "utilities.h"

void Scene::DrawGamePlay(Shader shader, std::list<Bullet::Bullet> bullets, std::list<Sugaroid::Sugaroid> sugaroids, Player::Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage)
{
	// Dibujar balas
	for (const auto& bullet : bullets)
	{
		DrawTexturePro(bulletsImage,
			Rectangle{ 0, 0, static_cast<float>(bulletsImage.width), static_cast<float>(bulletsImage.height) },  // Fuente (imagen completa)
			Rectangle{ bullet.position.x, bullet.position.y, bullet.size, bullet.size },  // Destino (posición y tamaño)
			Vector2{ bullet.radius, bullet.radius },  // Offset de origen para el centro
			0.0f,
			WHITE);
	}

	if (player.invisibility <= 0)
		DrawTexturePro(
			playerImage,  // La textura original
			Rectangle{ 0, 0, static_cast<float>(playerImage.width), static_cast<float>(playerImage.height) },  // Fuente: toda la imagen original
			Rectangle{ player.pos.x, player.pos.y, player.size, player.size },  // Destino: posición y nuevo tamaño
			Vector2{ player.size / 2, player.size / 2 },  // Offset (centro) si es necesario
			player.angle,  // Rotación (0 si no quieres rotar)
			WHITE  // Color (generalmente WHITE para no aplicar ningún tinte)
		);
	else
	{
		BeginShaderMode(shader);
		DrawTexturePro(
			playerImage,  // La textura original
			Rectangle{ 0, 0, static_cast<float>(playerImage.width), static_cast<float>(playerImage.height) },  // Fuente: toda la imagen original
			Rectangle{ player.pos.x, player.pos.y, player.size, player.size },  // Destino: posición y nuevo tamaño
			Vector2{ player.size / 2, player.size / 2 },  // Offset (centro) si es necesario
			player.angle,  // Rotación (0 si no quieres rotar)
			WHITE  // Color (generalmente WHITE para no aplicar ningún tinte)
		);
		EndShaderMode();
	}

	// Dibujar sugaroids
	for (const auto& sugaroid : sugaroids)
	{
		DrawTexturePro(
			sugaroidImage,
			Rectangle{ 0, 0, static_cast<float>(sugaroidImage.width), static_cast<float>(sugaroidImage.height) },  // Fuente (imagen completa)
			Rectangle{ sugaroid.position.x, sugaroid.position.y, sugaroid.size, sugaroid.size },  // Destino (posición y tamaño)
			Vector2{ sugaroid.radius, sugaroid.radius },  // Offset del centro
			0.0f,  // Rotación (puedes agregar rotación si es necesario)
			WHITE);
	}
}

void Scene::DrawPowerUpUnlockHud(PowerUpList unlockedPower, bool& levelUp, Font font)
{
	Vector2 mouse = GetMousePosition();
	Button button = {};

	Color outline = BLACK;
	Color semiTransparentBlack = { 0, 0, 0, 256 - 256 / 4 };

	float startX = (screenWidth - buttonWidth) / 2;
	float startY = screenHeight * 0.8f - buttonHeight / 2;

	button.rec = { startX, startY, buttonWidth, buttonHeight };
	button.option = Menus::Resume;

	std::string constText = "You unlocked: ";
	std::string powerUpName = "";
	std::string holeText = "";

	switch (unlockedPower)
	{
	case PowerUpList::BiggerBullets:
		powerUpName = "Bigger Bullets";
		break;
	case PowerUpList::X2BulletSpeed:
		powerUpName = "Bullets Speed x2";
		break;
	case PowerUpList::X3Bullets:
		powerUpName = "Burst Canyon";
		break;
	case PowerUpList::GuidedMissiles:
		powerUpName = "Guided Missiles";
		break;
	default:
		break;
	}

	holeText = constText + powerUpName + ".";

	if (Tools::CheckMouseButtonCollition(mouse, button.rec))
	{
		button.color = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? YELLOW : WHITE;

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			levelUp = false;
	}

	DrawRectangle(0, 0, screenWidth, screenHeight, semiTransparentBlack);

	DrawTextEx(font,
		holeText.c_str(),
		Vector2{ static_cast<float>(screenWidth / 2 - MeasureTextEx(font, holeText.c_str(), textFontSize, 0).x / 2), screenHeight / 2 - MeasureTextEx(font, holeText.c_str(), textFontSize, 0).y / 2 },
		textFontSize,
		0,
		WHITE);

	Tools::DrawButton(button.rec, "Resume", button.color, outline, font);
}

void Scene::DrawMainMenu(Menus& gameState, Font font, Texture2D gamesTitle)
{
	const int maxButtons = 5;
	Vector2 mouse;
	Button button[maxButtons] = {};

	float startX = (screenWidth - buttonWidth) / 2;
	float startY = screenHeight - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1));

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { startX, startY + i * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight };
	}

	button[0].option = Menus::Playing;
	button[1].option = Menus::Rules;
	button[2].option = Menus::Credits;
	button[3].option = Menus::WantToExit;

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

	DrawTexturePro(
		gamesTitle,
		Rectangle{ 0, 0, static_cast<float>(gamesTitle.width), static_cast<float>(gamesTitle.height) },  // Fuente: toda la imagen original
		Rectangle{ static_cast<float>(screenWidth) / 2 - gamesTitle.width / 2,  // Posición X centrada
				   static_cast<float>(screenHeight) / 3 - gamesTitle.height / 2,  // Posición Y centrada
				   static_cast<float>(gamesTitle.width),  // Ancho del título
				   static_cast<float>(gamesTitle.height) },  // Altura del título
		Vector2{ 0, 0 },  // Offset del centro
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
		case Menus::WantToExit:
			Tools::DrawButton(button[i].rec, "Exit", button[i].color, outline, font);
			break;
		default:
			break;
		}
	}
}

void Scene::DrawCredits(Font customFont)
{
	Button button = {};

	float newSmallFontSize = (10); // Ajustar tamaño de fuente
	float newtextFontSize = (textFontSize);  // Ajustar tamaño de fuente
	float newtitlesFontSize = (titlesFontSize); // Ajustar tamaño de fuente

	Vector2 titlePos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Credits", newtitlesFontSize, 2).x / 2, static_cast<float>(screenHeight) / 4 };
	Vector2 developerPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Developer: Lucio Stefano Piccioni.", newtextFontSize, 2).x / 2, titlePos.y + 60 };
	Vector2 musicPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Music:", newtextFontSize, 2).x / 2, static_cast<float>(screenHeight) / 2 - 60 };
	Vector2 music1Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "1. 'Game Over!' - Harris Cole", newSmallFontSize, 2).x / 2, musicPos.y + 30 };
	Vector2 music2Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", newSmallFontSize, 2).x / 2, music1Pos.y + 20 };
	Vector2 music3Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "3. 'Quiet Nights' - JEN", newSmallFontSize, 2).x / 2, music2Pos.y + 20 };
	Vector2 music4Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "4. 'Facade' - JEN", newSmallFontSize, 2).x / 2, music3Pos.y + 20 };
	Vector2 soundEffectsPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Sound Effects:", newtextFontSize, 2).x / 2, music4Pos.y + 30 };
	Vector2 chipTonePos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "ChipTone", newSmallFontSize, 2).x / 2, soundEffectsPos.y + 30 };
	Vector2 menuPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Press ESC to go back to the Menu.", newSmallFontSize, 2).x / 2, static_cast<float>(screenHeight) - 40 };

	button.rec.x += developerPos.x;
	button.rec.y = developerPos.y;

	DrawTextEx(customFont, "Credits", titlePos, newtitlesFontSize, 2, BLACK);
	DrawTextEx(customFont, "Developer: Lucio Stefano Piccioni.", developerPos, newtextFontSize, 2, BLACK);
	DrawTextEx(customFont, "Music:", musicPos, newtextFontSize, 2, BLACK);
	DrawTextEx(customFont, "1. 'Game Over!' - Harris Cole", music1Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(customFont, "2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", music2Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(customFont, "3. 'Quiet Nights' - JEN", music3Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(customFont, "4. 'Facade' - JEN", music4Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(customFont, "Sound Effects:", soundEffectsPos, newtextFontSize, 2, BLACK);
	DrawTextEx(customFont, "ChipTone", chipTonePos, newSmallFontSize, 2, BLACK);
	DrawTextEx(customFont, "Press ESC to go back to the Menu.", menuPos, newSmallFontSize, 2, BLACK);
}


void Scene::DrawGameRules(Font customFont)
{
	ClearBackground(RAYWHITE);

	Vector2 titlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Game Rules", titlesFontSize, 2).x / 2,
		static_cast<float>(screenHeight) / 10
	};

	Vector2 controlsTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Controls:", textFontSize, 2).x / 2,
		titlePos.y + titlesFontSize + buttonSpacing
	};

	Vector2 upArrowPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Up Arrow: Move Up", textFontSize, 2).x / 2,
		controlsTitlePos.y + buttonSpacing
	};

	Vector2 downArrowPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Down Arrow: Move Down", textFontSize, 2).x / 2,
		upArrowPos.y + buttonSpacing
	};

	Vector2 leftArrowPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Left Arrow: Move Left", textFontSize, 2).x / 2,
		downArrowPos.y + buttonSpacing
	};

	Vector2 rightArrowPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Right Arrow: Move Right", textFontSize, 2).x / 2,
		leftArrowPos.y + buttonSpacing
	};

	Vector2 firePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Left Click: Fire", textFontSize, 2).x / 2,
		rightArrowPos.y + buttonSpacing
	};

	Vector2 escPausePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "ESC: Pause", textFontSize, 2).x / 2,
		firePos.y + buttonSpacing
	};

	Vector2 points1Pos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Points for destroying sugaroids: 25", textFontSize, 2).x / 2,
		escPausePos.y + buttonSpacing
	};

	Vector2 points2Pos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Points for avoiding sugaroids: 5", textFontSize, 2).x / 2,
		points1Pos.y + buttonSpacing
	};

	Vector2 lives1Pos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "You start with 3 lives.", textFontSize, 2).x / 2,
		points2Pos.y + buttonSpacing
	};

	Vector2 lives2Pos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Lose all lives to end the game.", textFontSize, 2).x / 2,
		lives1Pos.y + buttonSpacing
	};

	Vector2 backToMenuPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(customFont, "Press ESC to return to menu", textFontSize, 2).x / 2,
		static_cast<float>(screenHeight) - 60
	};

	// Dibuja el texto en las posiciones calculadas
	DrawTextEx(customFont, "Game Rules", titlePos, titlesFontSize, 2, BLACK);
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


void Scene::DrawGameOver(Menus& gameState, Font font)
{
	const int maxButtons = 3;
	Vector2 mouse = GetMousePosition();
	Button button[maxButtons] = {};

	Color outline = BLACK;

	float startX = (static_cast<float>(screenWidth) - buttonWidth) / 2;
	float startY = ((static_cast<float>(screenHeight) - screenHeight / 5) - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1)));

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { startX, startY + i * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight };
	}

	button[0].option = Menus::Replay;
	button[1].option = Menus::MainMenu;
	button[2].option = Menus::WantToExit;

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

	Vector2 gameOverTextSize = MeasureTextEx(font, "GAME OVER", titlesFontSize, 0);  // Escalar el tamaño del texto
	Vector2 gameOverPos = { static_cast<float>(screenWidth) / 2 - gameOverTextSize.x / 2, static_cast<float>(screenHeight) / 3 };

	DrawTextEx(font, "GAME OVER", gameOverPos, titlesFontSize, 0, RED);  // Escalar el tamaño del texto

	for (int i = 0; i < maxButtons; i++)
	{
		switch (button[i].option)
		{
		case Menus::Replay:
			Tools::DrawButton(button[i].rec, "Replay", button[i].color, outline, font);
			break;
		case Menus::MainMenu:
			Tools::DrawButton(button[i].rec, "Main Menu", button[i].color, outline, font);
			break;
		case Menus::WantToExit:
			Tools::DrawButton(button[i].rec, "Exit", button[i].color, outline, font);
			break;
		}
	}
}


void Scene::DrawConfirmExit(Menus& gameState, Font font, Menus previusMenu)
{
	const int maxButtons = 2;

	Color outLine = BLACK;

	Vector2 mouse = GetMousePosition();
	Button button[maxButtons];

	float startX, startY;

	startX = (screenWidth - buttonWidth) / 2;
	startY = (screenHeight - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1))) / 2;

	button[0].option = Menus::ConfirmExit;
	button[1].option = Menus::CancelExit;

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { static_cast<float>(startX), static_cast<float>(startY + i * (buttonHeight + buttonSpacing)), buttonWidth, buttonHeight };

		switch (button[i].option)
		{
		case Menus::ConfirmExit:
			button[i].color = RED;
			break;

		case Menus::CancelExit:
			button[i].color = GREEN;
			break;

		default:
			break;
		}
	}

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

	switch (gameState)
	{
	case Menus::ConfirmExit:
		gameState = Menus::Exit;
		break;

	case Menus::CancelExit:
		gameState = previusMenu;
		timmerToCleanBuffer = 0.01f;
		break;

	default:
		break;
	}

	DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 0, 0, 0, 125 });

	DrawTextEx(font, "Are you sure you want to exit?",
		Vector2{ static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Are you sure you want to exit?", textFontSize, 0).x / 2, static_cast<float>(screenHeight / 4) },
		textFontSize, 0, outLine);

	for (int i = 0; i < maxButtons; i++)
	{
		switch (button[i].option)
		{
		case Menus::ConfirmExit:
			Tools::DrawButton(button[i].rec, "Yes", button[i].color, outLine, font);
			break;

		case Menus::CancelExit:
			Tools::DrawButton(button[i].rec, "No", button[i].color, outLine, font);
			break;

		default:
			break;
		}
	}
}


