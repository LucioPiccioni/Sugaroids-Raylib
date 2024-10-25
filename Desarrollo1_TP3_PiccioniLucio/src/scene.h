#pragma once

#include "raylib.h"
#include "menus.h"
#include "utilities.h"
#include "player.h"
#include "bullet.h"
#include "sugaroid.h"
#include <list>

namespace Scene
{
	void DrawCredits(int screenWidth, int screenHeight, Font font);
	void DrawGamePlay(std::list<Bullet::Bullet> bullets, std::list<Sugaroid::Sugaroid> sugaroids, Player::Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage);
	void DrawPowerUpUnlockHud(PowerUpList unlockedPower, bool levelUp, Font font, int screenWidth, int screenHeight);
	void DrawMainMenu(Menus& selectedOption, Font& font, Texture2D gamesTitle, int screenWidth, int screenHeight);
	void DrawGameRules(int screenWidth, int screenHeight, Font customFont);
	void DrawGameOver(Menus& gameState, Font font, int screenWidth, int screenHeight);
	void DrawConfirmExit(Font font, int screenWidth, int screenHeight);
}