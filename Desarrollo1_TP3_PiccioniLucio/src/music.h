#pragma once
#include "raylib.h"

struct GameMusic
{
	Music creditsMusic = LoadMusicStream("../res/music/Game Over! - Harris Cole.mp3");
	Music mainMenuMusic = LoadMusicStream("../res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	Music gameOverMusic = LoadMusicStream("../res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	Music gamePlayMusic = LoadMusicStream("../res/music/JEN - FACADE - soulmate.mp3");
};