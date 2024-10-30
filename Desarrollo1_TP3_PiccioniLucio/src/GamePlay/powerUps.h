#pragma once

enum class PowerUpList
{
	BiggerBullets,
	X2BulletSpeed,
	X3Bullets,
	GuidedMissiles,

	None
};

struct PowerUps
{
	bool biggerBullets = false;
	bool x2BulletSpeed = false;
	bool x3Bullets = false;
	bool guidedMissiles = false;
};