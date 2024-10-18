#include "sounds.h"

void Sounds::init(GameSounds& music)
{
	music.hurtSound = LoadSound("../res/soundEffects/hurt.wav");
	music.boomSound = LoadSound("../res/soundEffects/boom.wav");
	music.dieSound = LoadSound("../res/soundEffects/die.wav");
	music.shootSound = LoadSound("../res/soundEffects/shoot.wav");
}
