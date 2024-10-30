#include "sugaroid.h"

#include "utilities.h"

Sugaroid::Sugaroid Sugaroid::CreateEnemy(Vector2 playerPosition)
{
	int edge = rand() % 4 + 1;
	Vector2 spawnPosition = { 0, 0 };

	switch (edge)
	{
	case 1:  // Borde superior
		spawnPosition.x = static_cast<float>(rand() % screenWidth);
		spawnPosition.y = 0.0f;
		break;
	case 2:  // Borde inferior
		spawnPosition.x = static_cast<float>(rand() % screenWidth);
		spawnPosition.y = static_cast<float>(screenHeight);
		break;
	case 3:  // Borde izquierdo
		spawnPosition.x = 0.0f;
		spawnPosition.y = static_cast<float>(rand() % screenHeight);
		break;
	case 4:  // Borde derecho
		spawnPosition.x = static_cast<float>(screenWidth);
		spawnPosition.y = static_cast<float>(rand() % screenHeight);
		break;
	}

	Sugaroid newEnemy = {};
	Vector2 direction = { 0,0 };
	float speed = 0;
	float size = 0;

	float angle = rand() % 360 * (1800.f / PI);

	switch (Enemies(rand() % 2))
	{
	case Enemies::Sugaroid:

		size = static_cast<float>(rand() % 33 + 32);  // Tamaño entre 32 y 64
		speed = static_cast<float>(rand() % 201 + 100);  // Velocidad entre 100 y 200
		direction = { playerPosition.x - spawnPosition.x, playerPosition.y - spawnPosition.y };
		newEnemy.whichEnemy = Enemies::Sugaroid;

		break;

	case Enemies::Cometkie:

		size = static_cast<float>(rand() % 33 + 64);  // Tamaño entre 64 y 96
		speed = static_cast<float>(rand() % 51 + 50);  // Velocidad entre 50 y 100
		direction = { static_cast<float>(rand() % 201 - 100), static_cast<float>(rand() % 201 - 100) };
		newEnemy.whichEnemy = Enemies::Cometkie;

		break;

	default:
		break;
	}


	// Calcular la velocidad normalizada
	float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
	newEnemy.velocity = { 0.0f, 0.0f };
	if (length > 0)
	{
		newEnemy.velocity.x = (direction.x / length) * speed;
		newEnemy.velocity.y = (direction.y / length) * speed;
	}

	// Asignar propiedades comunes
	newEnemy.position = spawnPosition;
	newEnemy.size = size;
	newEnemy.radius = size / 2.0f;
	newEnemy.radius = size / 2.0f;
	newEnemy.toDestroy = false;
	newEnemy.angle = angle;
	newEnemy.mother = true;

	return newEnemy;
}


void Sugaroid::Spawner(float& sugaroidSpawnTimer, float sugaroidsSpawnRate, float deltaTime, Vector2 playerPos, std::list<Sugaroid>& sugaroids)
{
	sugaroidSpawnTimer += deltaTime;
	if (sugaroidSpawnTimer > sugaroidsSpawnRate)
	{
		sugaroids.push_back(CreateEnemy(playerPos));
		sugaroidSpawnTimer = 0;
	}
}

void Sugaroid::Movement(Sugaroid& sugaroid, float& deltaTime)
{
	sugaroid.position.x += sugaroid.velocity.x * deltaTime;
	sugaroid.position.y += sugaroid.velocity.y * deltaTime;
}
