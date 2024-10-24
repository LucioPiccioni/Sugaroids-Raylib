#pragma once

#include "sugaroid.h"
#include "player.h"
#include "menus.h"
#include "bullet.h"
#include <list>

namespace GameManager
{
	void ResetGame(std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, int& points);
	void ShouldResetMatch(Menus& gameState, Player::Player& player, std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, int& points);
	void BulletActions(std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float& deltaTime);
	void SugaroidsActions(std::list<Sugaroid::Sugaroid>& sugaroids, std::list<Bullet::Bullet>& bullets, Sound& hurtSound, float& deltaTime, float& points, int& score, Player::Player& player);

	void DificultyIncreas(float& sugaroidsSpawnTime);

	void PowerUnlockerLogic(PowerUps& boosts, PowerUpList& powerUpUnlocked, float& sugaroidsSpawnTime);

	PowerUpList PowerUpRulete();

	bool AreAllPowerUpsUnlocked(PowerUps& boosts);
	bool ShouldAddPowerUps(float& points);
	bool DidPlayerDied(Player::Player& player);
}