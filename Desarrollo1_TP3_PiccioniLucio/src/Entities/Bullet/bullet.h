#pragma once

#include <list>

#include "raylib.h"

#include "Entities/Enemies/sugaroid.h"
#include "GamePlay/powerUps.h"

namespace Bullet
{
	struct Bullet
	{
		Vector2 position = {};
		Vector2 velocity = {};
		float speed = 0;
		float radius = 0;
		bool toDestroy = false;
		float size = 32;

		Sugaroid::Sugaroid* targetedSugaroid = nullptr;
	};

	Bullet SpawnBullet(Vector2 playerPos, float playerAngle, PowerUps powerUps, std::list<Sugaroid::Sugaroid>& sugaroids);
	void Movement(Bullet& bullet, float deltaTime);
}