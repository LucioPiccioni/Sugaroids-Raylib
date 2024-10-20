#pragma once
#include "raylib.h"
#include <vector>

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


	void Constructor(Vector2& playerPosition, std::vector<Sugaroid>& sugaroids);
	void Spawner(float& spawnTimer, float& deltaTime,Vector2& playerPos, std::vector<Sugaroid>& sugaroids);
	void Movement(Sugaroid& sugaroid, float& deltaTime);
}

extern float sugaroidsSpawnRate;
