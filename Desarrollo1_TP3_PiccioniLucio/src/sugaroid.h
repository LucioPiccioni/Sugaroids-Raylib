#pragma once
#include "raylib.h"
#include <list>

namespace Sugaroid
{
	struct Sugaroid
	{

		Vector2 position = {};
		Vector2 velocity = {};
		float radius = 0;
		bool toDestroy = false;
		float size = 32;
		bool didItHitPlayer = false;
		bool bigSugaroid = false;
		bool outOfScreen = false;
	};


	void Constructor(Vector2& playerPosition, std::list<Sugaroid>& sugaroids, int screenWidth, int screenHeight);
	void Spawner(float& spawnTimer, float& sugaroidsSpawnRate, float& deltaTime, Vector2& playerPos, std::list<Sugaroid>& sugaroids, int screenWidth, int screenHeight);
	void Movement(Sugaroid& sugaroid, float& deltaTime);
}
