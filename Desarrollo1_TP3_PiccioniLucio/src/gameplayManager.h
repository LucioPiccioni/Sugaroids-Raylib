#pragma once

#include "sugaroid.h"
#include "player.h"
#include "menus.h"
#include "bullet.h"
#include <vector>

namespace GameManager
{
	void ResetGame(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, int& points);
	void ShouldResetMatch(Menus& gameState, Player::Player& player, std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, int& points);
	void BulletActions(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float& deltaTime);
	void SugaroidsActions(std::vector<Sugaroid::Sugaroid>& sugaroids, std::vector<Bullet::Bullet>& bullets, Sound& hurtSound, float& deltaTime, float& points, int& score, Player::Player& player);

	void DificultyIncreas(float& sugaroidsSpawnTime);

	void PowerUnlockerLogic(PowerUps& boosts, float& sugaroidsSpawnTime);

	PowerUpList PowerUpRulete();

	bool ShouldAddPowerUps(float& points);
	bool DidPlayerDied(Player::Player& player);
}