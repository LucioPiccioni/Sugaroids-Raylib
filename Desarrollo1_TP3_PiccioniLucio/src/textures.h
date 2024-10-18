#pragma once
#include "raylib.h"

namespace Textures
{
	struct GameTextures
	{
		Texture2D playerImage = {};
		Texture2D sugaroidImage = {};
		Texture2D bulletsImage = {};
		Texture2D backgroundImage = {};
		Texture2D gamesTitle = {};
	};

	void init(GameTextures& textures);
}