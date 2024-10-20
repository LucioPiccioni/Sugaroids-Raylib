#pragma once

#include "menus.h"
#include "raylib.h"
#include "player.h"
#include "music.h"
#include "sounds.h"
#include "textures.h"
#include "bullet.h"
#include "sugaroid.h"
#include <vector>

namespace EventManager
{
	void InitProgram();
	void ProgramLoop();
	void InitAssets(SoundTracks::GameMusic& music, Textures::GameTextures& textures, Sounds::GameSounds& sounds);
	void UnloadAssets(SoundTracks::GameMusic& music, Font& font,Textures::GameTextures& textures, Sounds::GameSounds& sounds);


	void MusicControl(Menus& gameState, SoundTracks::GameMusic& music, bool& gameOver);
}

extern Menus gameState;
extern int buttonSelected;