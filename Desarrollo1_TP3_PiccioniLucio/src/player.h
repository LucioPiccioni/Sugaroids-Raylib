#pragma once
#include "raylib.h"

struct Player
{
	int lives = 3;
	
	Vector2 pos = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
	Vector2 speed = {};
	Vector2 targetPos = {};
	Vector2 directionVector;
	float directionLength = 0;
	float maxSpeed = 300.0f;
	
	float size = 64;
	float angle = 0;

};

