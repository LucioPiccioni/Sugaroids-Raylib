#include "gameplayManager.h"
#include "utilities.h"

void GameManager::ShouldResetMatch(Menus& gameState, Player::Player& player, std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, int& points)
{
	switch (gameState)
	{
	case Menus::MainMenu:

		GameManager::ResetGame(bullets, sugaroids, player, gameOver, points);
		break;

	case Menus::Replay:

		GameManager::ResetGame(bullets, sugaroids, player, gameOver, points);
		gameState = Menus::Playing;
		break;

	case Menus::Exit:
		break;
	default:
		break;
	}

}

void GameManager::ResetGame(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, int& points)
{
	sugaroids.clear();
	bullets.clear();

	points = 0;

	player = Player::Player{};

	gameOver = false;
}

void GameManager::BulletActions(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float& deltaTime)
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

void GameManager::SugaroidsActions(std::vector<Sugaroid::Sugaroid>& sugaroids, std::vector<Bullet::Bullet>& bullets, Sound& hurtSound, float& deltaTime, float& playerEXP, int& score , Player::Player& player)
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
			playerEXP += 25;
			score += 25;
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

void GameManager::DificultyIncreas(float& sugaroidsSpawnTime)
{
	sugaroidsSpawnTime -= 0.25f;
}

void GameManager::PowerUnlockerLogic(PowerUps& boosts, float& sugaroidsSpawnTime)
{
	PowerUpList RandomPowerUp = PowerUpList::None;
	bool reRol = false;

	do
	{
		reRol = false;
		RandomPowerUp = PowerUpRulete();

		switch (RandomPowerUp)
		{
		case PowerUpList::BiggerBullets:

			if (boosts.biggerBullets)
				reRol = true;
			else
				boosts.biggerBullets = true;;
			break;

		case PowerUpList::X2BulletSpeed:

			if (boosts.x2BulletSpeed)
				reRol = true;
			else
				boosts.x2BulletSpeed = true;
			break;

		case PowerUpList::X3Bullets:

			if (boosts.x3Bullets)
				reRol = true;
			else
				boosts.x3Bullets = true;
			break;

		case PowerUpList::GuidedMissiles:

			if (boosts.guidedMissiles)
				reRol = true;
			else
				boosts.guidedMissiles = true;
			break;

		case PowerUpList::None:
			break;

		default:
			break;
		}

	} while (RandomPowerUp == PowerUpList::None || reRol);
}

PowerUpList GameManager::PowerUpRulete()
{
	switch (PowerUpList(rand() % 5))
	{
	case PowerUpList::BiggerBullets:

		return PowerUpList::BiggerBullets;
		break;

	case PowerUpList::X2BulletSpeed:

		return PowerUpList::X2BulletSpeed;
		break;

	case PowerUpList::X3Bullets:

		return PowerUpList::X3Bullets;
		break;

	case PowerUpList::GuidedMissiles:

		return PowerUpList::GuidedMissiles;
		break;

	default:

		return PowerUpList::None;
		break;
	}

}

bool GameManager::ShouldAddPowerUps(float& points)
{
	if (points >= 500)
	{
		points = 0;
		return true;
	}
	else
		return false;
}

bool GameManager::DidPlayerDied(Player::Player& player)
{
	return player.lives <= 0;
}