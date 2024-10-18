#pragma once

struct GameSounds
{
	Sound hurtSound = LoadSound("../res/soundEffects/hurt.wav");
	Sound boomSound = LoadSound("../res/soundEffects/boom.wav");
	Sound dieSound = LoadSound("../res/soundEffects/die.wav");
	Sound shootSound = LoadSound("../res/soundEffects/shoot.wav");
};