#include "textures.h"

void Textures::init(GameTextures& textures)
{
	textures.playerImage = LoadTexture("../res/sprites/player/spaceship.png");
	textures.sugaroidImage = LoadTexture("../res/sprites/enemies/sugaroid.png");
	textures.bulletsImage = LoadTexture("../res/sprites/bullets/star.png");
	textures.backgroundImage = LoadTexture("../res/backgrounds/pacific.png");
	textures.gamesTitle = LoadTexture("../res/title.png");
}
