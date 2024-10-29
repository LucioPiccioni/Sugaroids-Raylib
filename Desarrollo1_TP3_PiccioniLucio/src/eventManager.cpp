#include "eventManager.h"
#include "utilities.h"
#include "gameplayManager.h"
#include "button.h"
#include "scene.h"
#include <ctime>

void Engine::InitProgram()
{
#pragma warning(disable:4244)
	srand(time(NULL));
#pragma warning(default:4244)

	InitWindow(screenWidth, screenHeight, "Sugaroids");

	SetTargetFPS(144);

	SetExitKey(0);

	InitAudioDevice();

	ProgramExecutionAndLoop();

	CloseAudioDevice();
	CloseWindow();
}

void Engine::InitAssets(SoundTracks::GameMusic& music, Textures::GameTextures& textures, Sounds::GameSounds& sounds)
{
	Sounds::init(sounds);
	SoundTracks::init(music);
	Textures::init(textures);
}

void Engine::ProgramExecutionAndLoop()
{
	Menus gameState = Menus::MainMenu;

	bool gameOver = false;
	float deltaTime;
	Player::Player player;
	int points = 0;

	float sugaroidsSpawnRate = 1;
	float spawnTimer = 0;
	Vector2 mouse = { 0,0 };

	Font font = LoadFontEx("../res/fonts/rubikBubbles/RubikBubbles-Regular.ttf", static_cast<int>(titlesFontSize), 0, 0);

	SoundTracks::GameMusic music = {};
	Textures::GameTextures textures = {};
	Sounds::GameSounds sounds = {};

	InitAssets(music, textures, sounds);

	std::list<Bullet::Bullet> bullets;
	std::list<Sugaroid::Sugaroid> sugaroids;
	std::list<Sugaroid::Sugaroid> sugaroidsChildList;

	Menus previousMenu = Menus::MainMenu;

	bool pause = false;
	bool allBoostsUnlocked = false;

	int newScreenWidth = 1200;
	int newScreenHeight = 800;

	while (!WindowShouldClose() && gameState != Menus::Exit)
	{
		deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_DOWN))
		{
			newScreenWidth--;
			newScreenHeight--;
		}
		else if (IsKeyDown(KEY_UP))
		{
			newScreenWidth++;
			newScreenHeight++;
		}

		SetWindowSize(newScreenWidth, newScreenHeight);

		float scaleFactorX = static_cast<float>(newScreenWidth / screenWidth);
		float scaleFactorY = static_cast<float>(newScreenHeight / screenHeight);
		float scaleFactor = std::min(scaleFactorX, scaleFactorY);

		Engine::MusicControl(gameState, music, gameOver);

		switch (gameState)
		{
		case Menus::MainMenu:

			previousMenu = Menus::MainMenu;

			if (IsKeyPressed(KEY_ESCAPE))
				gameState = Menus::WantToExit;
			break;

		case Menus::Playing:

			previousMenu = Menus::Playing;

			if (!gameOver)
			{

				if (!pause && !player.levelingUp)
				{
					GameManager::SugaroidDestroyer(sugaroids, sugaroidsChildList, bullets, player.EXP, points);

					GameManager::SpawnAsteroidsChilds(sugaroids, sugaroidsChildList);

					allBoostsUnlocked = GameManager::AreAllPowerUpsUnlocked(player.boost);

					mouse = GetMousePosition();

					player.angle = atan2f(mouse.y - player.pos.y, mouse.x - player.pos.x) * (180.0f / PI);

					Player::Movement(player, deltaTime, newScreenWidth, newScreenHeight);

					Player::Shoot(player, sounds.shoot, bullets, sugaroids);

					Sugaroid::Spawner(spawnTimer, sugaroidsSpawnRate, deltaTime, player.pos, sugaroids, newScreenWidth, newScreenHeight);

					GameManager::SugaroidsActions(sugaroids, sounds.hurt, player, deltaTime, newScreenWidth, newScreenHeight);

					GameManager::BulletActions(bullets, sugaroids, sounds.boom, deltaTime, newScreenWidth, newScreenHeight);

					gameOver = GameManager::DidPlayerDied(player);

					if (!allBoostsUnlocked)
						player.levelingUp = GameManager::ShouldAddPowerUps(player.EXP);
				}

				if (!gameOver && player.EXP >= 500 && !allBoostsUnlocked)
				{
					GameManager::PowerUnlockerLogic(player.boost, player.lastPowerUnlock);
					player.EXP = 0;
					player.level++;
					GameManager::DificultyIncreas(sugaroidsSpawnRate);
					StopSound(sounds.levelUp);
					PlaySound(sounds.levelUp);
				}
				else if (!gameOver && player.EXP >= 500 && allBoostsUnlocked)
				{
					player.EXP = 0;
					player.level++;
					GameManager::DificultyIncreas(sugaroidsSpawnRate);
					StopSound(sounds.levelUp);
					PlaySound(sounds.levelUp);
				}

				if (IsKeyPressed(KEY_ESCAPE))
					pause = !pause;
			}

			break;

		case Menus::Rules:
		case Menus::Credits:
		case Menus::WantToExit:

			if (IsKeyPressed(KEY_ESCAPE))
				gameState = Menus::MainMenu;

			ConfirmExit(gameState, previousMenu, newScreenWidth, newScreenHeight, scaleFactor);

			break;

		default:
			break;
		}


		BeginDrawing();

		ClearBackground(BLACK);

		std::string pointsText = "Points: " + std::to_string(points);
		std::string playerLives = "Lives: " + std::to_string(player.lives);

		DrawTexturePro(
			textures.backgroundImage,
			Rectangle{ 0.0f, 0.0f, static_cast<float>(textures.backgroundImage.width), static_cast<float>(textures.backgroundImage.height) },
			Rectangle{ 0.0f, 0.0f, static_cast<float>(newScreenWidth), static_cast<float>(newScreenHeight) },
			Vector2{ 0, 0 },
			0.0f,
			WHITE);

		switch (gameState)
		{
		case Menus::MainMenu:

			if (timmerToCleanBuffer <= 0)
				Scene::DrawMainMenu(gameState, font, textures.gamesTitle, newScreenWidth, newScreenHeight, scaleFactor);
			else
				timmerToCleanBuffer -= 1 * deltaTime;
			break;

		case Menus::Playing:

			if (timmerToCleanBuffer <= 0)
			{
				if (gameOver)
				{
					Scene::DrawGameOver(gameState, font, newScreenWidth, newScreenHeight, scaleFactor);

					GameManager::ShouldResetMatch(gameState, player, bullets, sugaroids, gameOver, points, sugaroidsSpawnRate);
				}
				else
				{

					Scene::DrawGamePlay(bullets, sugaroids, player, textures.bulletsImage, textures.playerImage, textures.sugaroidImage);

					if (player.levelingUp && !allBoostsUnlocked)
					{
						Scene::DrawPowerUpUnlockHud(player.lastPowerUnlock, player.levelingUp, font, newScreenWidth, newScreenHeight, scaleFactor);
					}

					DrawTextEx(font, pointsText.c_str(), Vector2{ 0,0 }, scoreFontSize, 0, BLACK);
					DrawTextEx(font, playerLives.c_str(), Vector2{ 0, 20 }, scoreFontSize, 0, BLACK);
				}
			}
			else
				timmerToCleanBuffer -= 0.1f * deltaTime;

			break;

		case Menus::Rules:

			Scene::DrawGameRules(newScreenWidth, newScreenHeight, font, scaleFactor);
			break;

		case Menus::Credits:

			Scene::DrawCredits(newScreenWidth, newScreenHeight, font, scaleFactor);
			break;

		case Menus::WantToExit:

			Scene::DrawConfirmExit(gameState, font, previousMenu, newScreenWidth, newScreenHeight, scaleFactor);

			break;

		default:
			break;
		}

		EndDrawing();
	}

	UnloadAssets(music, font, textures, sounds);
}

void Engine::UnloadAssets(SoundTracks::GameMusic& music, Font& font, Textures::GameTextures& textures, Sounds::GameSounds& sounds)
{
	UnloadFont(font);

	UnloadTexture(textures.playerImage);
	UnloadTexture(textures.sugaroidImage);
	UnloadTexture(textures.backgroundImage);
	UnloadTexture(textures.bulletsImage);

	UnloadMusicStream(music.creditsMusic);
	UnloadMusicStream(music.mainMenuMusic);
	UnloadMusicStream(music.gameOverMusic);
	UnloadMusicStream(music.gamePlayMusic);
	UnloadSound(sounds.hurt);
	UnloadSound(sounds.boom);
	UnloadSound(sounds.die);
	UnloadSound(sounds.levelUp);
	UnloadSound(sounds.shoot);
}

void Engine::MusicControl(Menus& gameState, SoundTracks::GameMusic music, bool gameOver)
{
	const int AllMusic = 3;

	Music* actualMusic = nullptr;
	Music* stopMusic[AllMusic] = { nullptr };

	switch (gameState)
	{

	case Menus::MainMenu:

		SoundTracks::MainMenuOrganization(actualMusic, stopMusic, music);
		break;

	case Menus::Playing:

		if (gameOver)
		{
			SoundTracks::GameOverOrganization(actualMusic, stopMusic, music);
		}
		else
		{
			SoundTracks::GamePlayOrganization(actualMusic, stopMusic, music);
		}

		break;

	case Menus::Rules:

		SoundTracks::MainMenuOrganization(actualMusic, stopMusic, music);
		break;

	case Menus::Credits:

		SoundTracks::CreditsOrganization(actualMusic, stopMusic, music);
		break;

	case Menus::WantToExit:

		if (gameOver)
		{
			SoundTracks::GameOverOrganization(actualMusic, stopMusic, music);
		}
		else
		{
			SoundTracks::MainMenuOrganization(actualMusic, stopMusic, music);
		}
		break;

	case Menus::Exit:
		break;

	default:
		break;
	}

	for (int i = 0; i < AllMusic; i++)
	{
		if (stopMusic[i] != nullptr)
		{
			if (IsMusicStreamPlaying(*stopMusic[i]))
				StopMusicStream(*stopMusic[i]);
		}
	}

	if (actualMusic != nullptr)
	{
		if (!IsMusicStreamPlaying(*actualMusic))
			PlayMusicStream(*actualMusic);

		UpdateMusicStream(*actualMusic);
	}
}

void Engine::ConfirmExit(Menus& gameState, Menus previusMenu, int newScreenWidth, int newScreenHeight, float& scaleFactor)
{
	const int maxButtons = 2;

	Color outLine = BLACK;

	Vector2 mouse = GetMousePosition();
	Button button[maxButtons];

	float startX, startY;

	float newButtonWidth = buttonWidth * scaleFactor;
	float newButtonHeight = buttonHeight * scaleFactor;
	float newButtonSpacing = buttonSpacing * scaleFactor;

	startX = (newScreenWidth - newButtonWidth) / 2;
	startY = (newScreenHeight - (newButtonHeight * maxButtons + newButtonSpacing * scaleFactor * (maxButtons - 1))) / 2;

	button[0].option = Menus::ConfirmExit;
	button[1].option = Menus::CancelExit;

	for (int i = 0; i < maxButtons; i++)
	{
		button[i].rec = { startX, startY + i * (newButtonHeight + newButtonSpacing), static_cast<float>(newButtonHeight), static_cast<float>(newButtonHeight) };

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
}
