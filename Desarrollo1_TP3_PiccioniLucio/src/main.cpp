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

			Scene::DrawMainMenu(gameState, font, gamesTitle, screenWidth, screenHeight);
			break;

		case Menus::Playing:

			if (gameOver)
			{
				Scene::DrawGameOver(gameState, font, screenHeight, screenHeight);

				EventManager::ShouldResetMatch(gameState, player, bullets, sugaroids, gameOver);
			}
			else
			{
				std::string pointsText = "Points: " + std::to_string((int)points);

				DrawTextEx(font, pointsText.c_str(), Vector2{0,0}, scoreFontSize, 0, BLACK);

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