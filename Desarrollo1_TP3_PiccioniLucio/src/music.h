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
	void MainMenuOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic& music);
	void GamePlayOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic& music);
	void GameOverOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic& music);
	void CreditsOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic& music);
}