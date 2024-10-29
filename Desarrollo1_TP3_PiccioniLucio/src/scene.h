#pragma once

#include <list>

#include "raylib.h"

#include "menus.h"
#include "utilities.h"
#include "player.h"
#include "bullet.h"
#include "sugaroid.h"

namespace Scene
{
	void DrawCredits(int screenWidth, int screenHeight, Font font, float& scaleFactor);
	void DrawGamePlay(Shader shader, std::list<Bullet::Bullet> bullets, std::list<Sugaroid::Sugaroid> sugaroids, Player::Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage);
	void DrawPowerUpUnlockHud(PowerUpList unlockedPower, bool& levelUp, Font font, int screenWidth, int screenHeight, float& scaleFactor);
	void DrawMainMenu(Menus& selectedOption, Font font, Texture2D gamesTitle, int screenWidth, int screenHeight, float& scaleFactor);
	void DrawGameRules(int screenWidth, int screenHeight, Font customFont, float& scaleFactor);
	void DrawGameOver(Menus& gameState, Font font, int screenWidth, int screenHeight, float& scaleFactor);
	void DrawConfirmExit(Menus& gameState, Font font, Menus previusMenu, int screenWidth, int screenHeight, float& scaleFactor);
}