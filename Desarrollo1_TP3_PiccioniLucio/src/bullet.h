#pragma once
#include "raylib.h"
#include <vector>

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
	};

	void SpawnStarBullet(Vector2 playerPosition, float playerAngle, std::vector<Bullet>& bullets);
}