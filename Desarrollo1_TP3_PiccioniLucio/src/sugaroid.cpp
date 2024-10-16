#include "sugaroid.h"

void Sugaroid::Constructor(Vector2 playerPosition, std::vector<Sugaroid>& sugaroids, int screenWidth, int screenHeight)
{
	int edge = rand() % 4 + 1;
	Vector2 spawnPosition = { 0, 0 };

	switch (edge)
	{
	case 1:  // Top edge
		spawnPosition.x = static_cast<float>(rand() % screenWidth);
		spawnPosition.y = 0.0f;
		break;
	case 2:  // Bottom edge
		spawnPosition.x = static_cast<float>(rand() % screenWidth);
		spawnPosition.y = static_cast<float>(screenHeight);
		break;
	case 3:  // Left edge
		spawnPosition.x = 0.0f;
		spawnPosition.y = static_cast<float>(rand() % screenHeight);
		break;
	case 4:  // Right edge
		spawnPosition.x = static_cast<float>(screenWidth);
		spawnPosition.y = static_cast<float>(rand() % screenHeight);
		break;
	}

	float size = static_cast<float>(rand() % 33 + 32);  // Random size between 32 and 64
	float speed = static_cast<float>(rand() % 201 + 100);  // Random speed between 100 and 200

	Vector2 direction = { playerPosition.x - spawnPosition.x, playerPosition.y - spawnPosition.y };
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);

	Vector2 velocity = { 0.0f, 0.0f };
	if (length > 0)
	{
		velocity.x = (direction.x / length) * speed;
		velocity.y = (direction.y / length) * speed;
	}

	Sugaroid newSugaroid;
	newSugaroid.position = spawnPosition;
	newSugaroid.velocity = velocity;
	newSugaroid.size = size;
	newSugaroid.radius = size / 2.0f;
	newSugaroid.toDestroy = false;

	sugaroids.push_back(newSugaroid);
}

void Sugaroid::Spawner(float& spawnTimer, float& deltaTime, Player::Player& player, std::vector<Sugaroid>& sugaroids, int& screenWidth, int& screenHeight)
{
	spawnTimer += deltaTime;
	if (spawnTimer > 1)
	{
		Constructor(player.pos, sugaroids, screenWidth, screenHeight);
		spawnTimer = 0;
	}
}

void Sugaroid::Movement(Sugaroid& sugaroid, float& deltaTime)
{
	sugaroid.position.x += sugaroid.velocity.x * deltaTime;
	sugaroid.position.y += sugaroid.velocity.y * deltaTime;
}

void Sugaroid::ActionManager(std::vector<Sugaroid>& sugaroids, Sound& hurtSound, float& deltaTime, int& screenWidth, int& screenHeight, double& points, Player::Player& player)
{
	for (int i = 0; i < sugaroids.size(); )
	{
		Movement(sugaroids[i], deltaTime);

		if (CheckCollisionCircles(player.pos, player.size / 2, sugaroids[i].position, sugaroids[i].radius))
		{
			StopSound(hurtSound);
			PlaySound(hurtSound);

			sugaroids[i].toDestroy = true;
			player.lives--;
		}

		if (sugaroids[i].toDestroy ||
			static_cast<int>(sugaroids[i].position.x + sugaroids[i].radius) < 0 ||
			static_cast<int>(sugaroids[i].position.x - sugaroids[i].radius) > screenWidth ||
			static_cast<int>(sugaroids[i].position.y + sugaroids[i].radius) < 0 ||
			static_cast<int>(sugaroids[i].position.y - sugaroids[i].radius) > screenHeight)
		{
			if (!sugaroids[i].toDestroy)
				points += 5;
			else
				points += 25;

			sugaroids.erase(sugaroids.begin() + i);
		}
		else
		{
			i++;
		}
	}
}
