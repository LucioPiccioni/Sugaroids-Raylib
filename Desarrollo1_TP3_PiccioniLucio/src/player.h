#pragma once
#include "raylib.h"
#include "powerUps.h"
#include "sugaroid.h"
#include "utilities.h"
#include "bullet.h"
#include <list>

namespace Player
{
	struct Player
	{
		int lives = 3;

		Vector2 pos = { static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2) };
		Vector2 speed = {};
		Vector2 targetPos = {};
		Vector2 directionVector = {};
		float directionLength = 0;
		float maxSpeed = 300.0f;
		float size = 64;
		float radius = size / 2;
		float angle = 0;

		float invisibility = 0;

		int bulletBuffer = 0;
		float fireCooldown = 0;

		float EXP = 0;
		int level = 0;
		bool levelingUp = false;

		PowerUpList lastPowerUnlock = PowerUpList::None;

		PowerUps boost = {};
	};

	void Movement(Player& player, float deltaTime);
	void Shoot(Player& player, Sound shootSound, std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids);
}
