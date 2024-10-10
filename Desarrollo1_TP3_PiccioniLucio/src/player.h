#pragma once
#include "raylib.h"

struct Player
{
	Vector2 pos = {};
	int lives = 3;
	float maxSpeed = 400.0f;
	float width = 64 * 1.5;
	float height = 64 * 1.5;
	Vector2 speed = {};
	float velocity = 150.0f;
	float angle = 0;
	Vector2 targetPos = {};
};

