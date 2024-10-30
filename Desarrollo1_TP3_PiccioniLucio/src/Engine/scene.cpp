#include "scene.h"

#include "button.h"
#include "utilities.h"

void Scene::DrawGamePlay(Shader shader, std::list<Bullet::Bullet> bullets, std::list<Sugaroid::Sugaroid> sugaroids, Player::Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage, Texture2D cometkieImage, Texture2D chipImage)
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
	{
		DrawTexturePro(
			playerImage,  // La textura original
			Rectangle{ 0, 0, static_cast<float>(playerImage.width), static_cast<float>(playerImage.height) },  // Fuente: toda la imagen original
			Rectangle{ player.pos.x, player.pos.y, player.size, player.size },  // Destino: posición y nuevo tamaño
			Vector2{ player.size / 2, player.size / 2 },  // Offset (centro) si es necesario
			player.angle,  // Rotación (0 si no quieres rotar)
			WHITE  // Color (generalmente WHITE para no aplicar ningún tinte)
		);
	}
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
	for (auto& sugaroid : sugaroids)
	{
		switch (sugaroid.whichEnemy)
		{
		case Enemies::Sugaroid:

			DrawTexturePro(
				sugaroidImage,
				Rectangle{ 0, 0, static_cast<float>(sugaroidImage.width), static_cast<float>(sugaroidImage.height) },  // Fuente (imagen completa)
				Rectangle{ sugaroid.position.x, sugaroid.position.y, sugaroid.size, sugaroid.size },  // Destino (posición y tamaño)
				Vector2{ sugaroid.radius, sugaroid.radius },  // Offset del centro
				sugaroid.angle,
				WHITE);
			break;

		case Enemies::Cometkie:

			DrawTexturePro(
				cometkieImage,
				Rectangle{ 0, 0, static_cast<float>(cometkieImage.width), static_cast<float>(cometkieImage.height) },  // Fuente (imagen completa)
				Rectangle{ sugaroid.position.x, sugaroid.position.y, sugaroid.size, sugaroid.size },  // Destino (posición y tamaño)
				Vector2{ sugaroid.radius, sugaroid.radius },  // Offset del centro
				sugaroid.angle,
				WHITE);
			break;


		case Enemies::Chip:

			DrawTexturePro(
				chipImage,
				Rectangle{ 0, 0, static_cast<float>(chipImage.width), static_cast<float>(chipImage.height) },  // Fuente (imagen completa)
				Rectangle{ sugaroid.position.x, sugaroid.position.y, sugaroid.size, sugaroid.size },  // Destino (posición y tamaño)
				Vector2{ sugaroid.radius, sugaroid.radius },  // Offset del centro
				sugaroid.angle,
				WHITE);
			break;

		case Enemies::None:
			break;
		default:
			break;
		}
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

void Scene::DrawCredits(Menus& gameState, Font font)
{
	Button button = {};

	Vector2 mouse = GetMousePosition();

	float newSmallFontSize = 20.0f;
	float newTextFontSize = 25.0f;
	float newTitleFontSize = 30.0f;

	Vector2 titlePos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Credits", newTitleFontSize, 2).x / 2, static_cast<float>(screenHeight) / 6 };
	Vector2 developerPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Developer: Lucio Stefano Piccioni.", newTextFontSize, 2).x / 2, titlePos.y + 60 };
	Vector2 musicPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Music:", newTextFontSize, 2).x / 2, developerPos.y + 60 };
	Vector2 music1Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "1. 'Game Over!' - Harris Cole", newSmallFontSize, 2).x / 2, musicPos.y + 40 };
	Vector2 music2Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", newSmallFontSize, 2).x / 2, music1Pos.y + 30 };
	Vector2 music3Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "3. 'Quiet Nights' - JEN", newSmallFontSize, 2).x / 2, music2Pos.y + 30 };
	Vector2 music4Pos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "4. 'Facade' - JEN", newSmallFontSize, 2).x / 2, music3Pos.y + 30 };
	Vector2 soundEffectsPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Sound Effects:", newTextFontSize, 2).x / 2, music4Pos.y + 50 };
	Vector2 chipTonePos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "ChipTone", newSmallFontSize, 2).x / 2, soundEffectsPos.y + 30 };
	Vector2 menuPos = { static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Press ESC to go back to the Menu.", newSmallFontSize, 2).x / 2, static_cast<float>(screenHeight) - 40 };

	button.rec.width = buttonWidth;
	button.rec.height = buttonHeight;
	button.option = Menus::MainMenu;
	button.rec.x = static_cast<float>(screenWidth) / 2 - (button.rec.width / 2);

	button.rec.y = (chipTonePos.y + menuPos.y) / 2 - (button.rec.height / 2);

	DrawTextEx(font, "Credits", titlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Developer: Lucio Stefano Piccioni.", developerPos, newTextFontSize, 2, BLACK);
	DrawTextEx(font, "Music:", musicPos, newTextFontSize, 2, BLACK);
	DrawTextEx(font, "1. 'Game Over!' - Harris Cole", music1Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(font, "2. 'Falling Apart' - yawgooh (Lofi Girl Ambient)", music2Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(font, "3. 'Quiet Nights' - JEN", music3Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(font, "4. 'Facade' - JEN", music4Pos, newSmallFontSize, 2, BLACK);
	DrawTextEx(font, "Sound Effects:", soundEffectsPos, newTextFontSize, 2, BLACK);
	DrawTextEx(font, "ChipTone", chipTonePos, newSmallFontSize, 2, BLACK);
	DrawTextEx(font, "Press ESC to go back to the Menu.", menuPos, newSmallFontSize, 2, BLACK);

	Color outline = BLACK;

	if (Tools::CheckMouseButtonCollition(mouse, button.rec))
	{
		button.color = WHITE;

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			button.color = YELLOW;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			gameState = button.option;
	}

	std::string text = "Menu";
	Tools::DrawButton(button.rec, text, button.color, outline, font);
}

void Scene::DrawGameRules(Menus& gameState, Font font)
{
	float newtextFontSize = 25.0f;
	float newTitleFontSize = 30.0f;  // Tamaño de fuente para títulos más grandes

	Button button = {};
	Vector2 mouse = GetMousePosition();

	Vector2 titlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Game Rules", titlesFontSize, 2).x / 2,
		static_cast<float>(screenHeight) / 10
	};

	Vector2 controlsTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Controls:", newTitleFontSize, 2).x / 2,
		titlePos.y + titlesFontSize + buttonSpacing
	};

	Vector2 leftClickMovementPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Left Click: Move right", newtextFontSize, 2).x / 2,
		controlsTitlePos.y + buttonSpacing + titlesFontSize / 3
	};

	Vector2 firePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Left Click: Fire", newtextFontSize, 2).x / 2,
		leftClickMovementPos.y + buttonSpacing
	};

	Vector2 enemyTypesTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Enemy Types:", newTitleFontSize, 2).x / 2,
		firePos.y + buttonSpacing + newtextFontSize / 2
	};

	Vector2 sugaroidTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Sugaroids:", newTitleFontSize, 2).x / 2,
		enemyTypesTitlePos.y + buttonSpacing * 2 + newTitleFontSize / 2
	};

	Vector2 sugaroidInfoPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Fast enemies that split into 2 when destroyed. Worth 25 points.", newtextFontSize, 2).x / 2,
		sugaroidTitlePos.y + buttonSpacing + newTitleFontSize / 3
	};

	Vector2 cookieTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Cookies:", newTitleFontSize, 2).x / 2,
		sugaroidInfoPos.y + buttonSpacing * 2 + newtextFontSize / 2
	};

	Vector2 cookieInfoPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Give 10 points and explode into 4 chips.", newtextFontSize, 2).x / 2,
		cookieTitlePos.y + buttonSpacing
	};

	Vector2 chipTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Chips:", newTitleFontSize, 2).x / 2,
		cookieInfoPos.y + buttonSpacing * 2
	};

	Vector2 chipInfoPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Each chip is worth 4 points.", newtextFontSize, 2).x / 2,
		chipTitlePos.y + buttonSpacing
	};

	Vector2 powerUpTitlePos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Power-Ups:", newTitleFontSize, 2).x / 2,
		chipInfoPos.y + buttonSpacing * 2
	};

	Vector2 powerUpInfoPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Unlock a power-up every 500 points. The game gets harder!", newtextFontSize, 2).x / 2,
		powerUpTitlePos.y + buttonSpacing
	};

	Vector2 backToMenuPos = {
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Press ESC to return to menu", newtextFontSize, 2).x / 2,
		static_cast<float>(screenHeight) - 60
	};

	button.rec.width = buttonWidth;
	button.rec.height = buttonHeight;
	button.option = Menus::MainMenu;
	button.rec.x = static_cast<float>(screenWidth) / 2 - (button.rec.width / 2);
	button.rec.y = (backToMenuPos.y + powerUpInfoPos.y) / 2 - (button.rec.height / 2);

	DrawTextEx(font, "Game Rules", titlePos, titlesFontSize, 2, BLACK);
	DrawTextEx(font, "Controls:", controlsTitlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Left Click: Move right", leftClickMovementPos, newtextFontSize, 2, BLACK);
	DrawTextEx(font, "Left Click: Fire", firePos, newtextFontSize, 2, BLACK);
	DrawTextEx(font, "Enemy Types:", enemyTypesTitlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Sugaroids:", sugaroidTitlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Fast enemies that split into 2 when destroyed. Worth 25 points.", sugaroidInfoPos, newtextFontSize, 2, BLACK);
	DrawTextEx(font, "Cookies:", cookieTitlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Give 10 points and explode into 4 chips.", cookieInfoPos, newtextFontSize, 2, BLACK);
	DrawTextEx(font, "Chips:", chipTitlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Each chip is worth 4 points.", chipInfoPos, newtextFontSize, 2, BLACK);
	DrawTextEx(font, "Power-Ups:", powerUpTitlePos, newTitleFontSize, 2, BLACK);
	DrawTextEx(font, "Unlock a power-up every 500 points. The game gets harder!", powerUpInfoPos, newtextFontSize, 2, BLACK);
	DrawTextEx(font, "Press ESC to return to menu", backToMenuPos, newtextFontSize, 2, BLACK);

	Color outline = BLACK;

	if (Tools::CheckMouseButtonCollition(mouse, button.rec))
	{
		button.color = WHITE;

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			button.color = YELLOW;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			gameState = button.option;
	}

	std::string text = "Menu";
	Tools::DrawButton(button.rec, text, button.color, outline, font);
}


void Scene::DrawPauseMenu(Menus& gameState, Font font, bool& pause)
{
	const int buttonCount = 3;  // Cantidad de botones
	Vector2 mouse;
	Button buttons[buttonCount] = {}; // Array de botones

	// Inicializa la posición de los botones
	float startX = (screenWidth - buttonWidth) / 2;
	float startY = screenHeight - (buttonHeight * buttonCount + buttonSpacing * (buttonCount - 1));

	// Establece las opciones de los botones
	buttons[0].option = Menus::Resume;
	buttons[1].option = Menus::MainMenu;
	buttons[2].option = Menus::WantToExit;

	// Inicialización de los rectángulos de los botones
	for (int i = 0; i < buttonCount; i++)
	{
		buttons[i].rec = { startX, startY + i * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight };
	}

	Color outline = BLACK;
	mouse = GetMousePosition();

	// Dibuja los botones y gestiona la interacción
	for (int i = 0; i < buttonCount; i++)
	{
		if (Tools::CheckMouseButtonCollition(mouse, buttons[i].rec))
		{
			buttons[i].color = WHITE;

			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				buttons[i].color = YELLOW;
			}

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				gameState = buttons[i].option;
				pause = false;
			}
		}
	}


	Color semiTransparentBlack = { 0, 0, 0, 150 };
	DrawRectangle(0, 0, screenWidth, screenHeight, semiTransparentBlack);

	Vector2 titlePos =
	{
		static_cast<float>(screenWidth) / 2 - MeasureTextEx(font, "Pause", titlesFontSize, 2).x / 2,
		static_cast<float>(screenHeight) / 5
	};

	Color pastelPurple = { 214, 196, 224, 255 };


	DrawTextEx(font, "Pause", titlePos, titlesFontSize, 2, pastelPurple);

	for (int i = 0; i < buttonCount; i++)
	{
		switch (buttons[i].option)
		{
		case Menus::Resume:
			Tools::DrawButton(buttons[i].rec, "Resume", buttons[i].color, outline, font);
			break;
		case Menus::MainMenu:
			Tools::DrawButton(buttons[i].rec, "Main Menu", buttons[i].color, outline, font);
			break;
		case Menus::WantToExit:
			Tools::DrawButton(buttons[i].rec, "Exit Game", buttons[i].color, outline, font);
			break;
		default:
			break;
		}
	}

	if (gameState == Menus::Resume)
	{
		gameState = Menus::Playing;
	}
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

	for (int i = 0; i < maxButtons; i++)
	{
		if (Tools::CheckMouseButtonCollition(mouse, button[i].rec))
		{
			button[i].color = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? YELLOW : WHITE;

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
				gameState = button[i].option;
		}
	}

	Vector2 gameOverTextSize = MeasureTextEx(font, "GAME OVER", titlesFontSize, 0);
	Vector2 gameOverPos = { static_cast<float>(screenWidth) / 2 - gameOverTextSize.x / 2, static_cast<float>(screenHeight) / 3 };

	DrawTextEx(font, "GAME OVER", gameOverPos, titlesFontSize, 0, RED);

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
