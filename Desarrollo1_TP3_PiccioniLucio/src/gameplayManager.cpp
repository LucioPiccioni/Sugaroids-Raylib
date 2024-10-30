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

	sugaroidsSpawnTime = 2;

	player = Player::Player{};

	gameOver = false;
}

void GameManager::BulletActions(std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float deltaTime)
{
	for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
	{
		Bullet::Movement(*bulletIt, deltaTime);

		if (Tools::CheckIfOutOfBounds(bulletIt->position, bulletIt->radius))
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

void GameManager::SugaroidsActions(std::list<Sugaroid::Sugaroid>& sugaroids, Sound& hurtSound, Player::Player& player, float deltaTime)
{
	for (auto sugaroidIt = sugaroids.begin(); sugaroidIt != sugaroids.end(); sugaroidIt++)
	{
		Sugaroid::Movement(*sugaroidIt, deltaTime);

		if (Tools::CheckIfOutOfBounds(sugaroidIt->position, sugaroidIt->radius))
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

			if (player.invisibility <= 0)
			{
				player.lives--;
				player.invisibility = 1;
			}
		}
	}
}

void GameManager::SugaroidDestroyer(Vector2 playerPos, std::list<Sugaroid::Sugaroid>& sugaroids, std::list<Sugaroid::Sugaroid>& childSugaroids, std::list<Bullet::Bullet>& bullets, float& playerEXP, int& score)
{
	for (auto sugaroidIt = sugaroids.begin(); sugaroidIt != sugaroids.end();)
	{
		if (sugaroidIt->toDestroy)
		{

			if (!sugaroidIt->didItHitPlayer && !sugaroidIt->outOfScreen)
			{
				switch (sugaroidIt->whichEnemy)
				{
				case Enemies::Sugaroid:

					playerEXP += 25;
					score += 25;
					break;

				case Enemies::Cometkie:

					playerEXP += 10;
					score += 10;
					break;

				case Enemies::Chip:

					playerEXP += 3;
					score += 3;
					break;

				case Enemies::None:
					break;
				default:
					break;
				}
			}

			if (sugaroidIt->mother && sugaroidIt->whichEnemy == Enemies::Sugaroid)
			{
				Sugaroid::Sugaroid child1 = *sugaroidIt;
				Sugaroid::Sugaroid child2 = *sugaroidIt;

				// Separación basada en el eje de velocidad dominante
				const float separationOffset = 30.0f;  // Ajusta el valor para más o menos separación

				if (fabs(sugaroidIt->velocity.x) > fabs(sugaroidIt->velocity.y))
				{
					// Si la velocidad en X es mayor, separar en el eje X
					child1.position.x += separationOffset;
					child2.position.x -= separationOffset;
				}
				else
				{
					// Si la velocidad en Y es mayor, separar en el eje Y
					child1.position.y += separationOffset;
					child2.position.y -= separationOffset;
				}

				// Modificación de las velocidades para diferenciar a los hijos
				child1.velocity.x += 0.1f;
				child2.velocity.x *= -0.1f;

				// Reducción del tamaño en un 20%
				child1.size *= 0.8f;
				child2.size *= 0.8f;

				// Actualización de los radios
				child1.radius = child1.size / 2;
				child2.radius = child2.size / 2;

				// Ambos hijos no serán madres
				child1.mother = false;
				child2.mother = false;

				// Calcular dirección y velocidad para child1
				Vector2 direction = { playerPos.x - child1.position.x, playerPos.y - child1.position.y };
				float speed = static_cast<float>(rand() % 51 + 100);  // Velocidad entre 50 y 100
				float length = sqrtf(direction.x * direction.x + direction.y * direction.y);

				if (length > 0)
				{
					child1.velocity.x = (direction.x / length) * speed;
					child1.velocity.y = (direction.y / length) * speed;
				}

				direction = { playerPos.x - child2.position.x, playerPos.y - child2.position.y };
				length = sqrtf(direction.x * direction.x + direction.y * direction.y);

				if (length > 0)
				{
					child2.velocity.x = (direction.x / length) * speed;
					child2.velocity.y = (direction.y / length) * speed;
				}

				// Definir que los hijos no deben ser destruidos inicialmente
				child1.toDestroy = false;
				child2.toDestroy = false;

				// Definir el tipo de enemigo para ambos hijos
				child1.whichEnemy = Enemies::Sugaroid;
				child2.whichEnemy = Enemies::Sugaroid;

				// Agregar los hijos a la lista temporal
				childSugaroids.push_back(child1);
				childSugaroids.push_back(child2);
			}
			else if (sugaroidIt->mother && sugaroidIt->whichEnemy == Enemies::Cometkie)
			{
				for (int i = 0; i < 4; ++i)
				{
					Sugaroid::Sugaroid childCometkie = *sugaroidIt;
					childCometkie.mother = false;
					childCometkie.toDestroy = false;

					childCometkie.size *= 0.6f;
					childCometkie.radius = childCometkie.size / 2;

					childCometkie.velocity.x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
					childCometkie.velocity.y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;

					float speedFactor = static_cast<float>(rand() % 50 + 50);  // Entre 50 y 100
					childCometkie.velocity.x *= speedFactor;
					childCometkie.velocity.y *= speedFactor;

					childCometkie.whichEnemy = Enemies::Chip;

					childSugaroids.push_back(childCometkie);
				}
			}

			// Desvincular el sugaroid de los proyectiles que lo apuntaban
			for (auto& bullet : bullets)
			{
				if (&(*sugaroidIt) == bullet.targetedSugaroid)
				{
					bullet.targetedSugaroid = nullptr;
				}
			}

			// Eliminar el sugaroid original de la lista
			sugaroidIt = sugaroids.erase(sugaroidIt);
		}
		else
		{
			++sugaroidIt;
		}
	}
}


void GameManager::SpawnAsteroidsChilds(std::list<Sugaroid::Sugaroid>& sugaroids, std::list<Sugaroid::Sugaroid>& childSugaroids)
{
	sugaroids.insert(sugaroids.end(), childSugaroids.begin(), childSugaroids.end());

	childSugaroids.clear();
}

void GameManager::DificultyIncreas(float& sugaroidsSpawnTime)
{
	sugaroidsSpawnTime -= sugaroidsSpawnTime * 0.1f;
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
