#include "eventManager.h"
#include "utilities.h"

void EventManager::MusicControl(Menus& gameState, Music& mainMenuMusic, Music& gamePlayMusic, Music& gameOverMusic, Music& creditsMusic, bool gameOver)
{
	const int AllMusic = 3;

	Music* actualMusic = &mainMenuMusic;
	Music* stopMusic[AllMusic] = { &mainMenuMusic, &gamePlayMusic,  &creditsMusic };

	switch (gameState)
	{
	case Menus::MainMenu:

		actualMusic = &mainMenuMusic;

		stopMusic[0] = &gamePlayMusic;
		stopMusic[1] = &gameOverMusic;
		stopMusic[2] = &creditsMusic;
		break;

	case Menus::Playing:

		switch (gameOver)
		{
		case true:

			actualMusic = &gameOverMusic;

			stopMusic[0] = &mainMenuMusic;
			stopMusic[1] = &gamePlayMusic;
			stopMusic[2] = &creditsMusic;
			break;

		case false:

			actualMusic = &gamePlayMusic;

			stopMusic[0] = &mainMenuMusic;
			stopMusic[1] = &gameOverMusic;
			stopMusic[2] = &creditsMusic;
			break;

		default:
			break;
		}

		break;

	case Menus::Rules:

		actualMusic = &mainMenuMusic;

		stopMusic[0] = &gamePlayMusic;
		stopMusic[1] = &gameOverMusic;
		stopMusic[2] = &creditsMusic;
		break;

	case Menus::Credits:

		actualMusic = &creditsMusic;

		stopMusic[0] = &gamePlayMusic;
		stopMusic[1] = &gameOverMusic;
		stopMusic[2] = &mainMenuMusic;
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

void EventManager::ShouldResetMatch(Menus& gameState, Player::Player& player, std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, double& points)
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

void EventManager::ResetGame(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, double& points)
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

void EventManager::SugaroidsActions(std::vector<Sugaroid::Sugaroid>& sugaroids, std::vector<Bullet::Bullet>& bullets, Sound& hurtSound, float& deltaTime, double& points, Player::Player& player)
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

bool EventManager::DidPlayerDied(Player::Player& player)
{
	return player.lives <= 0;
}
