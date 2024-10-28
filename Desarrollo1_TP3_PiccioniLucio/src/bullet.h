#pragma once
#include "raylib.h"
#include "sugaroid.h"
#include "powerUps.h"
#include <list>

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

	Bullet SpawnBullet(Vector2 playerPos, float playerAngle, PowerUps powerUps, std::list<Sugaroid::Sugaroid> sugaroids);
	void Movement(Bullet& bullet, float deltaTime);
}