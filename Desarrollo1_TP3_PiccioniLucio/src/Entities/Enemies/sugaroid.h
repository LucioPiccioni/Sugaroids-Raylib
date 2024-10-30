#pragma once

#include <list>

#include "raylib.h"

enum class Enemies
{
	Sugaroid,
	Cometkie,
	Chip,
	None
};

namespace Sugaroid
{
	struct Sugaroid
	{
		Enemies whichEnemy = Enemies::None;

		Vector2 position = {};
		Vector2 velocity = {};
		float radius = 0;
		float angle = 0;

		bool toDestroy = false;
		float size = 32;
		bool didItHitPlayer = false;
		bool bigSugaroid = false;
		bool outOfScreen = false;
		bool mother = false;
	};


	Sugaroid CreateEnemy(Vector2 playerPosition);
	void Spawner(float& spawnTimer, float sugaroidsSpawnRate, float deltaTime, Vector2 playerPos, std::list<Sugaroid>& sugaroids);
	void Movement(Sugaroid& sugaroid, float& deltaTime);
}
