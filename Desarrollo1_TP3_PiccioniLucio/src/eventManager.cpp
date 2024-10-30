#include "eventManager.h"

#include <ctime>

#include "utilities.h"
#include "gameplayManager.h"
#include "button.h"
#include "scene.h"

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

	float sugaroidsSpawnRate = 2;
	float spawnTimer = 0;
	Vector2 mouse = { 0,0 };

	Font font = LoadFontEx("../res/fonts/rubikBubbles/RubikBubbles-Regular.ttf", static_cast<int>(titlesFontSize), 0, 0);

	SoundTracks::GameMusic music = {};
	Textures::GameTextures textures = {};
	Sounds::GameSounds sounds = {};

	Shader shader = LoadShader(0, "../src/blink_shader.fs");

	InitAssets(music, textures, sounds);

	std::list<Bullet::Bullet> bullets;
	std::list<Sugaroid::Sugaroid> sugaroids;
	std::list<Sugaroid::Sugaroid> sugaroidsChildList;

	Menus previousMenu = Menus::MainMenu;

	bool pause = false;
	bool allBoostsUnlocked = false;

	float blinkFrequency = 0.5f;  // Frecuencia del parpadeo
	float blinkAmplitude = 1.5f;   // Amplitud del parpadeo

	while (!WindowShouldClose() && gameState != Menus::Exit)
	{
		double time = GetTime();

		SetShaderValue(shader, GetShaderLocation(shader, "time"), &time, SHADER_UNIFORM_FLOAT);
		SetShaderValue(shader, GetShaderLocation(shader, "blinkFrequency"), &blinkFrequency, SHADER_UNIFORM_FLOAT);
		SetShaderValue(shader, GetShaderLocation(shader, "blinkAmplitude"), &blinkAmplitude, SHADER_UNIFORM_FLOAT);

		deltaTime = GetFrameTime();

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
					GameManager::SugaroidDestroyer(player.pos, sugaroids, sugaroidsChildList, bullets, player.EXP, points);

					GameManager::SpawnAsteroidsChilds(sugaroids, sugaroidsChildList);

					allBoostsUnlocked = GameManager::AreAllPowerUpsUnlocked(player.boost);

					mouse = GetMousePosition();

					player.angle = atan2f(mouse.y - player.pos.y, mouse.x - player.pos.x) * (180.0f / PI);

					Player::Movement(player, deltaTime);

					Player::Shoot(player, sounds.shoot, bullets, sugaroids);

					Sugaroid::Spawner(spawnTimer, sugaroidsSpawnRate, deltaTime, player.pos, sugaroids);

					GameManager::SugaroidsActions(sugaroids, sounds.hurt, player, deltaTime);

					GameManager::BulletActions(bullets, sugaroids, sounds.boom, deltaTime);

					gameOver = GameManager::DidPlayerDied(player);

					if (!allBoostsUnlocked)
						player.levelingUp = GameManager::ShouldAddPowerUps(player.EXP);

					if (player.invisibility > 0)
						player.invisibility -= deltaTime;
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

			ConfirmExit(gameState, previousMenu);

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
			Rectangle{ 0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenWidth) },
			Vector2{ 0, 0 },
			0.0f,
			WHITE);

		switch (gameState)
		{
		case Menus::MainMenu:

			if (timmerToCleanBuffer <= 0)
				Scene::DrawMainMenu(gameState, font, textures.gamesTitle);
			else
				timmerToCleanBuffer -= 1 * deltaTime;
			break;

		case Menus::Playing:

			if (timmerToCleanBuffer <= 0)
			{
				if (gameOver)
				{
					Scene::DrawGameOver(gameState, font);

					GameManager::ShouldResetMatch(gameState, player, bullets, sugaroids, gameOver, points, sugaroidsSpawnRate);
				}
				else
				{
					if (pause)
						Scene::DrawPauseMenu(gameState, font, pause);
					{
						Scene::DrawGamePlay(shader, bullets, sugaroids, player, textures.bulletsImage, textures.playerImage, textures.sugaroidImage, textures.cometkieImage, textures.chipImage);

						if (player.levelingUp && !allBoostsUnlocked)
						{
							Scene::DrawPowerUpUnlockHud(player.lastPowerUnlock, player.levelingUp, font);
						}

						DrawTextEx(font, pointsText.c_str(), Vector2{ 0,0 }, scoreFontSize, 0, BLACK);
						DrawTextEx(font, playerLives.c_str(), Vector2{ 0, 20 }, scoreFontSize, 0, BLACK);
					}
				}
			}
			else
				timmerToCleanBuffer -= 0.1f * deltaTime;

			break;

		case Menus::Rules:

			Scene::DrawGameRules(gameState, font);
			break;

		case Menus::Credits:

			Scene::DrawCredits(gameState, font);
			break;

		case Menus::WantToExit:

			Scene::DrawConfirmExit(gameState, font, previousMenu);

			break;

		default:
			break;
		}

		EndDrawing();
	}

	UnloadShader(shader);
	UnloadAssets(music, font, textures, sounds);
}

void Engine::UnloadAssets(SoundTracks::GameMusic& music, Font& font, Textures::GameTextures& textures, Sounds::GameSounds& sounds)
{
	UnloadFont(font);

	UnloadTexture(textures.playerImage);
	UnloadTexture(textures.sugaroidImage);
	UnloadTexture(textures.cometkieImage);
	UnloadTexture(textures.chipImage);
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

void Engine::ConfirmExit(Menus& gameState, Menus previusMenu)
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
		button[i].rec = { startX, startY + i * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight };

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
