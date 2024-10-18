#include "eventManager.h"
#include "utilities.h"
#include "scene.h"
#include <ctime>

Menus gameState = Menus::MainMenu;
int buttonSelected = 0;

void EventManager::InitProgram()
{

	srand(time(nullptr));
	InitWindow(screenWidth, screenHeight, "Sugaroids");

	SetTargetFPS(144);

	SetExitKey(0);

	InitAudioDevice();

	ProgramLoop();

	CloseAudioDevice();
	CloseWindow();
}

void EventManager::InitAssets(SoundTracks::GameMusic& music, Textures::GameTextures& textures, Sounds::GameSounds& sounds)
{
	Sounds::init(sounds);
	SoundTracks::init(music);
	Textures::init(textures);
}

void EventManager::ProgramLoop()
{
	bool gameOver = false;
	float deltaTime;
	Player::Player player;
	int points = 0;
	float spawnTimer = 0;
	Vector2 mouse;

	Font font = LoadFontEx("../res/fonts/rubikBubbles/RubikBubbles-Regular.ttf", 40, 0, 0);

	SoundTracks::GameMusic music = {};
	Textures::GameTextures textures = {};
	Sounds::GameSounds sounds = {};

	InitAssets(music, textures, sounds);

	std::vector<Bullet::Bullet> bullets;
	std::vector<Sugaroid::Sugaroid> sugaroids;

	bool pause = false;

	while (!WindowShouldClose() && gameState != Menus::Exit)
	{
		deltaTime = GetFrameTime();

		EventManager::MusicControl(gameState, music, gameOver);

		switch (gameState)
		{
		case Menus::MainMenu:

			if (IsKeyPressed(KEY_ESCAPE))
				gameState = Menus::Exit;
			break;

		case Menus::Playing:

			if (!gameOver)
			{
				if (!pause)
				{
					mouse = GetMousePosition();

					player.angle = atan2f(mouse.y - player.pos.y, mouse.x - player.pos.x) * (180.0f / PI);

					Player::Movement(player, deltaTime, screenWidth, screenHeight);

					Player::Shoot(player, sounds.shootSound, bullets, sugaroids, deltaTime);

					Sugaroid::Spawner(spawnTimer, deltaTime, player.pos, sugaroids);

					EventManager::SugaroidsActions(sugaroids, bullets, sounds.hurtSound, deltaTime, points, player);

					EventManager::BulletActions(bullets, sugaroids, sounds.boomSound, deltaTime);

					gameOver = EventManager::DidPlayerDied(player);
				}

				if (IsKeyPressed(KEY_ESCAPE))
					pause = !pause;
			}

			break;

		case Menus::Rules:
		case Menus::Credits:
		case Menus::Exit:

			if (IsKeyPressed(KEY_ESCAPE))
				gameState = Menus::MainMenu;
			break;

		default:
			break;
		}


		BeginDrawing();

		ClearBackground(BLACK);

		std::string pointsText = "Points: " + std::to_string((int)points);
		std::string playerLives = "Lives: " + std::to_string(player.lives);

		DrawTexturePro(
			textures.backgroundImage,
			Rectangle{ 0, 0, (float)textures.backgroundImage.width, (float)textures.backgroundImage.height },
			Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
			Vector2{ 0, 0 },
			0.0f,
			WHITE);

		switch (gameState)
		{
		case Menus::MainMenu:

			Scene::DrawMainMenu(gameState, font, textures.gamesTitle, screenWidth, screenHeight);

			break;

		case Menus::Playing:


			switch (gameOver)
			{
			case true:

				Scene::DrawGameOver(gameState, font, screenWidth, screenHeight);

				EventManager::ShouldResetMatch(gameState, player, bullets, sugaroids, gameOver, points);

				break;

			case false:


				DrawTextEx(font, pointsText.c_str(), Vector2{ 0,0 }, scoreFontSize, 0, BLACK);
				DrawTextEx(font, playerLives.c_str(), Vector2{ 0, 20 }, scoreFontSize, 0, BLACK);

				Scene::DrawGamePlay(bullets, sugaroids, player, textures.bulletsImage, textures.playerImage, textures.sugaroidImage);

				break;

			default:
				break;
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

	UnloadAssets(music, font, textures, sounds);
}

void EventManager::UnloadAssets(SoundTracks::GameMusic& music, Font& font, Textures::GameTextures& textures, Sounds::GameSounds& sounds)
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
	UnloadSound(sounds.hurtSound);
	UnloadSound(sounds.boomSound);
	UnloadSound(sounds.dieSound);
	UnloadSound(sounds.shootSound);
}

void EventManager::MusicControl(Menus& gameState, SoundTracks::GameMusic& music, bool& gameOver)
{
	const int AllMusic = 3;

	Music* actualMusic = &music.mainMenuMusic;
	Music* stopMusic[AllMusic] = { &music.mainMenuMusic, &music.gamePlayMusic,  &music.creditsMusic };

	switch (gameState)
	{
	case Menus::MainMenu:

		actualMusic = &music.mainMenuMusic;

		stopMusic[0] = &music.gamePlayMusic;
		stopMusic[1] = &music.gameOverMusic;
		stopMusic[2] = &music.creditsMusic;
		break;

	case Menus::Playing:

		switch (gameOver)
		{
		case true:

			actualMusic = &music.gameOverMusic;

			stopMusic[0] = &music.mainMenuMusic;
			stopMusic[1] = &music.gamePlayMusic;
			stopMusic[2] = &music.creditsMusic;
			break;

		case false:

			actualMusic = &music.gamePlayMusic;

			stopMusic[0] = &music.mainMenuMusic;
			stopMusic[1] = &music.gameOverMusic;
			stopMusic[2] = &music.creditsMusic;
			break;

		default:
			break;
		}

		break;

	case Menus::Rules:

		actualMusic = &music.mainMenuMusic;

		stopMusic[0] = &music.gamePlayMusic;
		stopMusic[1] = &music.gameOverMusic;
		stopMusic[2] = &music.creditsMusic;
		break;

	case Menus::Credits:

		actualMusic = &music.creditsMusic;

		stopMusic[0] = &music.gamePlayMusic;
		stopMusic[1] = &music.gameOverMusic;
		stopMusic[2] = &music.mainMenuMusic;
		break;

	case Menus::Exit:
		break;

	default:
		break;
	}

	if (gameState != Menus::Exit)
	{
		if (!IsMusicStreamPlaying(*actualMusic))
			PlayMusicStream(*actualMusic);
	}

	if (gameState != Menus::Exit)
	{
		for (int i = 0; i < 3; i++)
		{
			if (IsMusicStreamPlaying(*stopMusic[i]))
				StopMusicStream(*stopMusic[i]);
		}
	}

	if (gameState != Menus::Exit)
		UpdateMusicStream(*actualMusic);
}

void EventManager::ShouldResetMatch(Menus& gameState, Player::Player& player, std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, int& points)
{
	switch (gameState)
	{
	case Menus::MainMenu:

		EventManager::ResetGame(bullets, sugaroids, player, gameOver, points);
		break;

	case Menus::Replay:

		EventManager::ResetGame(bullets, sugaroids, player, gameOver, points);
		gameState = Menus::Playing;
		break;

	case Menus::Exit:
		break;
	default:
		break;
	}

}

void EventManager::ResetGame(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, int& points)
{
	sugaroids.clear();
	bullets.clear();

	points = 0;

	player = Player::Player{};

	gameOver = false;
}

void EventManager::BulletActions(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float& deltaTime)
{
	float angleToSugaroid = 0;

	for (int i = 0; i < bullets.size(); )
	{
		Bullet::Movement(bullets[i], deltaTime);

		if (static_cast<int>(bullets[i].position.x + bullets[i].radius) < 0 ||
			static_cast<int>(bullets[i].position.x - bullets[i].radius) > screenWidth ||
			static_cast<int>(bullets[i].position.y + bullets[i].radius) < 0 ||
			static_cast<int>(bullets[i].position.y - bullets[i].radius) > screenHeight)
			bullets[i].toDestroy = true;


		for (int j = 0; j < sugaroids.size(); j++)
		{
			if (CheckCollisionCircles(bullets[i].position, bullets[i].radius, sugaroids[j].position, sugaroids[j].radius))
			{
				StopSound(boomSound);
				PlaySound(boomSound);

				sugaroids[j].toDestroy = true;
				bullets[i].toDestroy = true;

				for (Bullet::Bullet& bullet : bullets)
				{
					if (&sugaroids[j] == bullet.targetedSugaroid)
					{
						bullet.targetedSugaroid = nullptr;
					}
				}

				break;
			}
		}

		if (bullets[i].toDestroy)
			bullets.erase(bullets.begin() + i);
		else
			i++;
	}
}

void EventManager::SugaroidsActions(std::vector<Sugaroid::Sugaroid>& sugaroids, std::vector<Bullet::Bullet>& bullets, Sound& hurtSound, float& deltaTime, int& points, Player::Player& player)
{
	for (int i = 0; i < sugaroids.size(); )
	{
		if (static_cast<int>(sugaroids[i].position.x + sugaroids[i].radius) < 0 ||
			static_cast<int>(sugaroids[i].position.x - sugaroids[i].radius) > screenWidth ||
			static_cast<int>(sugaroids[i].position.y + sugaroids[i].radius) < 0 ||
			static_cast<int>(sugaroids[i].position.y - sugaroids[i].radius) > screenHeight)
		{
			sugaroids[i].outOfScreen = true;
			sugaroids[i].toDestroy = true;
		}

		Sugaroid::Movement(sugaroids[i], deltaTime);

		if (CheckCollisionCircles(player.pos, player.size / 2, sugaroids[i].position, sugaroids[i].radius))
		{
			StopSound(hurtSound);
			PlaySound(hurtSound);

			sugaroids[i].didItHitPlayer = true;
			sugaroids[i].toDestroy = true;
			player.lives--;
		}

		if (!sugaroids[i].didItHitPlayer && sugaroids[i].toDestroy && !sugaroids[i].outOfScreen)
		{
			points += 25;
		}

		if (sugaroids[i].toDestroy)
		{
			for (Bullet::Bullet& bullet : bullets)
			{
				if (&sugaroids[i] == bullet.targetedSugaroid)
				{
					bullet.targetedSugaroid = nullptr;
				}
			}

			sugaroids.erase(sugaroids.begin() + i);
		}
		else
			i++;
	}

}

bool EventManager::ShouldAddPowerUps(int& points)
{
	return points % 500 == 0;
}

bool EventManager::DidPlayerDied(Player::Player& player)
{
	return player.lives <= 0;
}
