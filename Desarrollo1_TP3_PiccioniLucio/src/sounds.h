#pragma once
#include "raylib.h"

namespace Sounds
{
	struct GameSounds
	{
		Sound hurtSound = {};
		Sound boomSound = {};
		Sound dieSound = {};
		Sound shootSound = {};
	};

	void init(GameSounds& music);
}