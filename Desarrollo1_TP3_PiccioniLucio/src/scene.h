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
	void DrawCredits(Font font);
	void DrawGamePlay(Shader shader, std::list<Bullet::Bullet> bullets, std::list<Sugaroid::Sugaroid> sugaroids, Player::Player player, Texture2D bulletsImage, Texture2D playerImage, Texture2D sugaroidImage);
	void DrawPowerUpUnlockHud(PowerUpList unlockedPower, bool& levelUp, Font font);
	void DrawMainMenu(Menus& gameState, Font font, Texture2D gamesTitle);
	void DrawGameRules(Font customFont);
	void DrawGameOver(Menus& gameState, Font font);
	void DrawConfirmExit(Menus& gameState, Font font, Menus previusMenu);
}