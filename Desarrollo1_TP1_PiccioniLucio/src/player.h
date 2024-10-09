#pragma once
#include "raylib.h"

struct Player
{
	Vector2 pos;
	int lives = 3;
	float width = 64 * 1.5;
	float height = 64 * 1.5;
	float speed = 150.0;
	float angle = 0;
};

