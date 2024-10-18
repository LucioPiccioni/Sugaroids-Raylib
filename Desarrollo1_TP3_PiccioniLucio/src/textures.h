#pragma once
#include "raylib.h"

struct GameTextures
{
	Texture2D playerImage = LoadTexture("../res/sprites/player/spaceship.png");
	Texture2D sugaroidImage = LoadTexture("../res/sprites/enemies/sugaroid.png");
	Texture2D bulletsImage = LoadTexture("../res/sprites/bullets/star.png");
	Texture2D backgroundImage = LoadTexture("../res/backgrounds/pacific.png");
	Texture2D gamesTitle = LoadTexture("../res/title.png");
};