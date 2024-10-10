#pragma once
#include "raylib.h"

struct Bullet
{
	Vector2 position;
	Vector2 velocity;
	float radius;
	bool toDestroy;
	float size = 32;
};