#include "eventManager.h"
#include "scene.h"
#include "player.h"
#include "sugaroid.h"
#include "bullet.h"
#include "raylib.h"
#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>

Menus gameState = Menus::MainMenu;
int buttonSelected = 0;

int main()
{
	int screenWidth = 800;
	int screenHeight = 600;

	srand(time(nullptr));
	InitWindow(screenWidth, screenHeight, "Sugaroids");
	SetTargetFPS(144);

	bool gameOver = false;
	float deltaTime;
	Player::Player player;
	double points = 0;
	float spawnTimer = 0;
	Vector2 mouse;

	InitAudioDevice();

	Font font = LoadFont("../res/fonts/rubikBubbles/RubikBubbles-Regular.ttf");

	Texture2D playerImage = LoadTexture("../res/sprites/player/spaceship.png");
	Texture2D sugaroidImage = LoadTexture("../res/sprites/enemies/sugaroid.png");
	Texture2D bulletsImage = LoadTexture("../res/sprites/bullets/star.png");

	Music creditsMusic = LoadMusicStream("../res/music/Game Over! - Harris Cole.mp3");
	Music mainMenuMusic = LoadMusicStream("../res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	Music gameOverMusic = LoadMusicStream("../res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	Music gamePlayMusic = LoadMusicStream("../res/music/JEN - FACADE - soulmate.mp3");

	Sound hurtSound = LoadSound("../res/soundEffects/hurt.wav");
	Sound boomSound = LoadSound("../res/soundEffects/boom.wav");
	Sound dieSound = LoadSound("../res/soundEffects/die.wav");
	Sound shootSound = LoadSound("../res/soundEffects/shoot.wav");

	std::vector<Bullet::Bullet> bullets;
	std::vector<Sugaroid::Sugaroid> sugaroids;

	Texture2D backgroundImage = LoadTexture("../res/backgrounds/pacific.png");

	Texture2D gamesTitle = LoadTexture("../res/title.png");

	while (!WindowShouldClose() && gameState != Menus::Exit)
	{
		deltaTime = GetFrameTime();
		
		EventManager::MusicControl(gameState, mainMenuMusic, gamePlayMusic, gameOverMusic, creditsMusic, gameOver);

		if (gameState == Menus::Playing && !gameOver)
		{
			mouse = GetMousePosition();

			player.angle = atan2f(mouse.y - player.pos.y, mouse.x - player.pos.x) * (180.0f / PI);

			Player::Movement(player, deltaTime, screenWidth, screenHeight);

			Player::Shoot(player, shootSound, bullets);

			Sugaroid::Spawner(spawnTimer, deltaTime, player, sugaroids, screenWidth, screenHeight);

			Sugaroid::ActionManager(sugaroids, hurtSound, deltaTime, screenWidth, screenHeight, points, player);

			EventManager::SugaroidBulletCollition(bullets, sugaroids, boomSound, deltaTime, screenWidth, screenHeight);

			gameOver = EventManager::DidPlayerDied(player);

		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTexturePro(
			backgroundImage,
			Rectangle{ 0, 0, (float)backgroundImage.width, (float)backgroundImage.height },
			Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
			Vector2{ 0, 0 },
			0.0f,
			WHITE);

		switch (gameState)
		{
		case Menus::MainMenu:

			Scene::DrawMainMenu(gameState, gamesTitle, screenWidth, screenHeight);
			break;

		case Menus::Playing:

			if (gameOver)
			{
				const int maxButtons = 3;

				int textWidth = MeasureText("GAME OVER", 20);

				Vector2 mouse;
				Button button[maxButtons];

				int startX, startY;

				startX = (screenWidth - buttonWidth) / 2;
				startY = (screenHeight - (buttonHeight * maxButtons + buttonSpacing * (maxButtons - 1)));

				for (int i = 0; i < maxButtons; i++)
				{
					button[i].rec = { static_cast<float>(startX), static_cast<float>(startY + i * (buttonHeight + buttonSpacing)), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
				}

				button[0].option = Menus::Replay;
				button[1].option = Menus::MainMenu;
				button[2].option = Menus::Exit;

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

				// Calcula el factor de escala basado en las proporciones de la pantalla y la imagen
				float scaleFactor = std::min((float)screenWidth / gamesTitle.width, (float)screenHeight / gamesTitle.height);

				// Calcula el nuevo tamaño del título basado en el factor de escala
				float newWidth = gamesTitle.width * scaleFactor;
				float newHeight = gamesTitle.height * scaleFactor;

				// Dibuja el título centrado y escalado

				DrawText("GAME OVER", (screenWidth - textWidth) / 2, screenHeight / 2, 20, RED);

				for (int i = 0; i < maxButtons; i++)
				{
					switch (button[i].option)
					{
					case Menus::Replay:

						Tools::DrawButton(button[i].rec, "Replay", button[i].color);
						break;

					case Menus::MainMenu:

						Tools::DrawButton(button[i].rec, "MainMenu", button[i].color);
						break;

					case Menus::Exit:

						Tools::DrawButton(button[i].rec, "Exit", button[i].color);
						break;

					default: break;
					}
				}

				switch (gameState)
				{
				case Menus::Replay:

					EventManager::ResetGame(bullets, sugaroids, player, gameOver);
					gameState = Menus::Playing;
					break;

				case Menus::MainMenu:
					break;
				case Menus::Exit:
					break;
				default:
					break;
				}

			}
			else
			{
				Scene::DrawGamePlay(bullets, sugaroids, player, bulletsImage, playerImage, sugaroidImage);
			}

			break;

		case Menus::Rules:

			Scene::DrawGameRules(screenWidth, screenHeight, font);
			break;

		case Menus::Credits:

			Scene::DrawCredits(screenWidth, screenHeight, font);
			break;

		case Menus::Exit:
			break;

		default:
			break;
		}

		EndDrawing();
	}

	UnloadFont(font);

	UnloadTexture(playerImage);
	UnloadTexture(sugaroidImage);
	UnloadTexture(backgroundImage);
	UnloadTexture(bulletsImage);

	UnloadMusicStream(creditsMusic);
	UnloadMusicStream(mainMenuMusic);
	UnloadMusicStream(gameOverMusic);
	UnloadMusicStream(gamePlayMusic);
	UnloadSound(hurtSound);
	UnloadSound(boomSound);
	UnloadSound(dieSound);
	UnloadSound(shootSound);

	CloseAudioDevice();
	CloseWindow();

	return 0;
}