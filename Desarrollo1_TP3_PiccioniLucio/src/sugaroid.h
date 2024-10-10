#pragma once
#include "raylib.h"

struct Sugaroid
{
	Vector2 position;
	Vector2 velocity;
	float radius;
	bool toDestroy;
	float size = 32;
};