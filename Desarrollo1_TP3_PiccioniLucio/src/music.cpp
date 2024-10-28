#include "music.h"

void SoundTracks::init(GameMusic& music)
{
	music.creditsMusic = LoadMusicStream("../res/music/Game Over! - Harris Cole.mp3");
	music.mainMenuMusic = LoadMusicStream("../res/music/yawgooh - falling apart - Lofi Girl Ambient.mp3");
	music.gameOverMusic = LoadMusicStream("../res/music/JEN - QUIET NIGHTS - soulmate.mp3");
	music.gamePlayMusic = LoadMusicStream("../res/music/JEN - FACADE - soulmate.mp3");
}

void SoundTracks::MainMenuOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic music)
{
	actualMusic = &music.mainMenuMusic;

	stopMusic[0] = &music.gamePlayMusic;
	stopMusic[1] = &music.gameOverMusic;
	stopMusic[2] = &music.creditsMusic;
}

void SoundTracks::GamePlayOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic music)
{
	actualMusic = &music.gamePlayMusic;

	stopMusic[0] = &music.mainMenuMusic;
	stopMusic[1] = &music.gameOverMusic;
	stopMusic[2] = &music.creditsMusic;
}

void SoundTracks::GameOverOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic music)
{
	actualMusic = &music.gameOverMusic;

	stopMusic[0] = &music.mainMenuMusic;
	stopMusic[1] = &music.gamePlayMusic;
	stopMusic[2] = &music.creditsMusic;
}

void SoundTracks::CreditsOrganization(Music*& actualMusic, Music* stopMusic[], GameMusic music)
{
	actualMusic = &music.creditsMusic;

	stopMusic[0] = &music.gamePlayMusic;
	stopMusic[1] = &music.gameOverMusic;
	stopMusic[2] = &music.mainMenuMusic;
}
