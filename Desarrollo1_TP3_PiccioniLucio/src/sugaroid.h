#pragma once
#include "raylib.h"
#include "player.h"
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
	};

	void Constructor(Vector2 playerPosition, std::vector<Sugaroid>& sugaroids, int screenWidth, int screenHeight);
	void Spawner(float& spawnTimer, float& deltaTime, Player::Player& player, std::vector<Sugaroid>& sugaroids, int& screenWidth, int& screenHeight);
	void Movement(Sugaroid& sugaroid, float& deltaTime);
	void ActionManager(std::vector<Sugaroid>& sugaroids, Sound& hurtSound, float& deltaTime, int& screenWidth, int& screenHeight, double& points, Player::Player& player);
}