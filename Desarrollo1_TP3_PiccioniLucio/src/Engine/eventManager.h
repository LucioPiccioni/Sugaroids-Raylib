#pragma once

#include "Engine/menus.h"
#include "raylib.h"
#include "Entities/Player/player.h"
#include "Assets/Music/music.h"
#include "Assets/Sounds/sounds.h"
#include "Assets/Textures/textures.h"
#include "Entities/Bullet/bullet.h"
#include "Entities/Enemies/sugaroid.h"
#include <vector>

namespace Engine
{
	void InitProgram();
	void ProgramExecutionAndLoop();
	void InitAssets(SoundTracks::GameMusic& music, Textures::GameTextures& textures, Sounds::GameSounds& sounds);
	void UnloadAssets(SoundTracks::GameMusic& music, Font& font, Textures::GameTextures& textures, Sounds::GameSounds& sounds);


	void MusicControl(Menus& gameState, SoundTracks::GameMusic music, bool gameOver);

	void ConfirmExit(Menus& gameState, Menus previusMenu);
}