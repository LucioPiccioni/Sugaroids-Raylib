#pragma once
#include "raylib.h"

namespace SoundTracks
{
	struct GameMusic
	{
		Music creditsMusic = {};
		Music mainMenuMusic = {};
		Music gameOverMusic = {};
		Music gamePlayMusic = {};
	};

	void init(GameMusic& music);
}