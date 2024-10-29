#include "sugaroid.h"
#include "utilities.h"

void Sugaroid::Constructor(Vector2& playerPosition, std::list<Sugaroid>& sugaroids, int newScreenWidth, int newScreenHeight)
{
	int edge = rand() % 4 + 1;
	Vector2 spawnPosition = { 0, 0 };

	switch (edge)
	{
	case 1:  // Top edge
		spawnPosition.x = static_cast<float>(rand() % newScreenWidth);
		spawnPosition.y = 0.0f;
		break;
	case 2:  // Bottom edge
		spawnPosition.x = static_cast<float>(rand() % newScreenWidth);
		spawnPosition.y = static_cast<float>(newScreenHeight);
		break;
	case 3:  // Left edge
		spawnPosition.x = 0.0f;
		spawnPosition.y = static_cast<float>(rand() % newScreenHeight);
		break;
	case 4:  // Right edge
		spawnPosition.x = static_cast<float>(newScreenWidth);
		spawnPosition.y = static_cast<float>(rand() % newScreenHeight);
		break;
	}

	float size = static_cast<float>(rand() % 33 + 32);  // Random size between 32 and 64
	float speed = static_cast<float>(rand() % 201 + 100);  // Random speed between 100 and 200

	Vector2 direction = { playerPosition.x - spawnPosition.x, playerPosition.y - spawnPosition.y };
	float length = sqrtf(direction.x * direction.x + direction.y * direction.y);

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
	newSugaroid.mother = true;

	sugaroids.push_back(newSugaroid);
}


void Sugaroid::Spawner(float& spawnTimer, float& sugaroidsSpawnRate, float& deltaTime, Vector2& playerPos, std::list<Sugaroid>& sugaroids, int newScreenWidth, int newScreenHeight)
{
	spawnTimer += deltaTime;
	if (spawnTimer > sugaroidsSpawnRate)
	{
		Constructor(playerPos, sugaroids, newScreenWidth, newScreenHeight);
		spawnTimer = 0;
	}
}


void Sugaroid::Movement(Sugaroid& sugaroid, float& deltaTime)
{
	sugaroid.position.x += sugaroid.velocity.x * deltaTime;
	sugaroid.position.y += sugaroid.velocity.y * deltaTime;
}
