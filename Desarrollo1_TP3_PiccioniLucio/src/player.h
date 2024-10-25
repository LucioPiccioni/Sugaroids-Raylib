#pragma once
#include "raylib.h"
#include "powerUps.h"
#include "sugaroid.h"
#include "bullet.h"
#include <list>

namespace Player
{
	struct Player
	{
		int lives = 3;

		Vector2 pos = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
		Vector2 speed = {};
		Vector2 targetPos = {};
		Vector2 directionVector = {};
		float directionLength = 0;
		float maxSpeed = 300.0f;
		float size = 64;
		float angle = 0;

		int bulletBuffer = 0;
		float fireCooldown = 0;

		float EXP = 0;
		int level = 0;
		bool levelingUp = false;

		PowerUpList lastPowerUnlock = PowerUpList::None;

		PowerUps boost = {};
	};

	void Movement(Player& player, float deltaTime, int screenWidth, int screenHeight);
	void Shoot(Player& player, Sound& shootSound, std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids);
}
