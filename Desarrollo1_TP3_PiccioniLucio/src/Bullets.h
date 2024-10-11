#pragma once
#include "raylib.h"

struct Bullet
{
	Vector2 position;
	Vector2 velocity;
	float speed = 0;
	float radius = 0;
	bool toDestroy = false;
	float size = 32;
};