#pragma once
#include "raylib.h"
#include "bullet.h"
#include <vector>

namespace Player
{
	struct Player
	{
		int lives = 3;

		Vector2 pos = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
		Vector2 speed = {};
		Vector2 targetPos = {};
		Vector2 directionVector;
		float directionLength = 0;
		float maxSpeed = 300.0f;

		float size = 64;
		float angle = 0;

	};

	void Movement(Player& player, float deltaTime, int screenWidth, int screenHeight);
	void Shoot(Player& player, Sound& shootSound, std::vector<Bullet::Bullet>& bullets);
}
