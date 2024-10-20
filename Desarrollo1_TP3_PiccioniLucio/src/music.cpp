#include "music.h"

void SoundTracks::init(GameMusic& music)
{
	music.creditsMusic = LoadMusicStream("../res/music/Game Over! - Harris Cole.mp3");
	music.mainMenuMusic = LoadMusicStream("../res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	music.gameOverMusic = LoadMusicStream("../res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	music.gamePlayMusic = LoadMusicStream("../res/music/JEN - FACADE - soulmate.mp3");
}