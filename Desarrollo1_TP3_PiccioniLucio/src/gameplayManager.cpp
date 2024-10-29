#include "gameplayManager.h"
#include "utilities.h"

void GameManager::ShouldResetMatch(Menus& gameState, Player::Player& player, std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, int& points, float& sugaroidsSpawnTime)
{
	switch (gameState)
	{
	case Menus::MainMenu:
		GameManager::ResetGame(bullets, sugaroids, player, gameOver, points, sugaroidsSpawnTime);
		break;

	case Menus::Replay:
		GameManager::ResetGame(bullets, sugaroids, player, gameOver, points, sugaroidsSpawnTime);
		gameState = Menus::Playing;
		break;

	case Menus::Exit:
		break;
	default:
		break;
	}
}

void GameManager::ResetGame(std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, int& points, float& sugaroidsSpawnTime)
{
	sugaroids.clear();
	bullets.clear();

	points = 0;

	sugaroidsSpawnTime = 1;

	player = Player::Player{};

	gameOver = false;
}

void GameManager::BulletActions(std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float deltaTime, int newScreenWidth, int newScreenHeight)
{
	for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
	{
		Bullet::Movement(*bulletIt, deltaTime);

		if (Tools::CheckIfOutOfBounds(bulletIt->position, bulletIt->radius, newScreenWidth, newScreenHeight))
			bulletIt->toDestroy = true;

		for (auto& sugaroid : sugaroids)
		{
			if (Tools::CheckCollisionCircles(bulletIt->position, bulletIt->radius, sugaroid.position, sugaroid.radius))
			{
				StopSound(boomSound);
				PlaySound(boomSound);

				sugaroid.toDestroy = true;
				bulletIt->toDestroy = true;

				break;
			}
		}

		if (bulletIt->toDestroy)
			bulletIt = bullets.erase(bulletIt);
		else
			bulletIt++;
	}
}

void GameManager::SugaroidsActions(std::list<Sugaroid::Sugaroid>& sugaroids, Sound& hurtSound, Player::Player& player, float deltaTime, int newScreenWidth, int newScreenHeight)
{
	for (auto sugaroidIt = sugaroids.begin(); sugaroidIt != sugaroids.end(); sugaroidIt++)
	{
		Sugaroid::Movement(*sugaroidIt, deltaTime);

		if (Tools::CheckIfOutOfBounds(sugaroidIt->position, sugaroidIt->radius, newScreenWidth, newScreenHeight))
		{
			sugaroidIt->outOfScreen = true;
			sugaroidIt->toDestroy = true;
		}

		if (Tools::CheckCollisionCircles(player.pos, player.size / 2, sugaroidIt->position, sugaroidIt->radius))
		{
			StopSound(hurtSound);
			PlaySound(hurtSound);

			sugaroidIt->didItHitPlayer = true;
			sugaroidIt->toDestroy = true;
			player.lives--;
		}
	}
}

void GameManager::SugaroidDestroyer(std::list<Sugaroid::Sugaroid>& sugaroids, std::list<Sugaroid::Sugaroid>& childSugaroids, std::list<Bullet::Bullet>& bullets, float& playerEXP, int& score)
{
	for (auto sugaroidIt = sugaroids.begin(); sugaroidIt != sugaroids.end();)
	{
		if (sugaroidIt->toDestroy)
		{
			if (!sugaroidIt->didItHitPlayer && sugaroidIt->toDestroy && !sugaroidIt->outOfScreen)
			{
				playerEXP += 25;
				score += 25;
			}

			if (sugaroidIt->mother && !sugaroidIt->didItHitPlayer)
			{
				// Create two new sugaroids
				Sugaroid::Sugaroid child1 = *sugaroidIt;
				Sugaroid::Sugaroid child2 = *sugaroidIt;

				// Modify their properties to differentiate them
				child1.velocity.x += 0.1f; // Slightly change direction
				child2.velocity.x *= -0.1f; // Slightly change direction
				child1.mother = false;
				child2.mother = false;
				child1.toDestroy = false;
				child2.toDestroy = false;

				// Add the new sugaroids to the temporary list
				childSugaroids.push_back(child1);
				childSugaroids.push_back(child2);
			}

			for (auto& bullet : bullets)
			{
				if (&(*sugaroidIt) == bullet.targetedSugaroid)
				{
					bullet.targetedSugaroid = nullptr;
				}
			}

			sugaroidIt = sugaroids.erase(sugaroidIt);
		}
		else
			sugaroidIt++;
	}
}

void GameManager::SpawnAsteroidsChilds(std::list<Sugaroid::Sugaroid>& sugaroids, std::list<Sugaroid::Sugaroid>& childSugaroids)
{
	sugaroids.insert(sugaroids.end(), childSugaroids.begin(), childSugaroids.end());

	childSugaroids.clear();
}

void GameManager::DificultyIncreas(float& sugaroidsSpawnTime)
{
	sugaroidsSpawnTime -= sugaroidsSpawnTime * 0.15f;
}

void GameManager::PowerUnlockerLogic(PowerUps& boosts, PowerUpList& powerUpUnlocked)
{
	powerUpUnlocked = PowerUpList::None;
	bool reRol = false;

	do
	{
		reRol = false;
		powerUpUnlocked = PowerUpRulete();

		switch (powerUpUnlocked)
		{
		case PowerUpList::BiggerBullets:
			if (boosts.biggerBullets)
				reRol = true;
			else
				boosts.biggerBullets = true;
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
			reRol = true;
			break;

		default:
			reRol = true;
			break;
		}

	} while (powerUpUnlocked == PowerUpList::None || reRol);
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

bool GameManager::AreAllPowerUpsUnlocked(PowerUps& boosts)
{
	return boosts.biggerBullets && boosts.x2BulletSpeed && boosts.x3Bullets && boosts.guidedMissiles;
}

bool GameManager::ShouldAddPowerUps(float& playerEXP)
{
	return playerEXP >= 500;
}

bool GameManager::DidPlayerDied(Player::Player& player)
{
	return player.lives <= 0;
}
