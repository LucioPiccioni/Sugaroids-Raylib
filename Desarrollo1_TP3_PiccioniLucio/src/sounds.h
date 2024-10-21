#pragma once
#include "raylib.h"

namespace Sounds
{
	struct GameSounds
	{
		Sound hurt = {};
		Sound boom = {};
		Sound die = {};
		Sound shoot = {};
		Sound levelUp = {};
	};

	void init(GameSounds& music);
}