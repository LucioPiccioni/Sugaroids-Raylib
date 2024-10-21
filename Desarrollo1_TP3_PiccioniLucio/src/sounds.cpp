#include "sounds.h"

void Sounds::init(GameSounds& sound)
{
	sound.hurt = LoadSound("../res/soundEffects/hurt.wav");
	sound.boom = LoadSound("../res/soundEffects/boom.wav");
	sound.die = LoadSound("../res/soundEffects/die.wav");
	sound.shoot = LoadSound("../res/soundEffects/shoot.wav");
	sound.levelUp = LoadSound("../res/soundEffects/levelUp.wav");
}
