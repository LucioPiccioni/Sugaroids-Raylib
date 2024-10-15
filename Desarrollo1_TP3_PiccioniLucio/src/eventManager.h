#pragma once

#include "menus.h"
#include "raylib.h"
#include "player.h"
#include "bullet.h"
#include "sugaroid.h"
#include <vector>


namespace EventManager
{
	void MusicControl(Menus& gameState, Music& mainMenuMusic, Music& gamePlayMusic, Music& gameOverMusic, Music& creditsMusic, bool gameOver);
	void ShouldResetMatch(Menus& gameState, Player::Player& player, std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, bool& gameOver);
	void ResetGame(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver);
	void SugaroidBulletCollition(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float& deltaTime, int& screenWidth, int& screenHeight);

	bool DidPlayerDied(Player::Player& player);
}