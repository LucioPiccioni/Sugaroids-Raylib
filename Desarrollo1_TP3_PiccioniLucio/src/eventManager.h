#pragma once

#include "menus.h"
#include "raylib.h"
#include "player.h"
#include "music.h"
#include "sounds.h"
#include "textures.h"
#include "bullet.h"
#include "sugaroid.h"
#include <vector>

namespace EventManager
{
	void InitProgram();
	void ProgramLoop();
	void UnloadAssets(GameMusic& music, Font& font, GameTextures& textures, GameSounds& sounds);


	void MusicControl(Menus& gameState, GameMusic& music, bool& gameOver);
	void ShouldResetMatch(Menus& gameState, Player::Player& player, std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, bool& gameOver, int& points);
	void ResetGame(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Player::Player& player, bool& gameOver, int& points);
	void BulletActions(std::vector<Bullet::Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids, Sound& boomSound, float& deltaTime);
	void SugaroidsActions(std::vector<Sugaroid::Sugaroid>& sugaroids, std::vector<Bullet::Bullet>& bullets, Sound& hurtSound, float& deltaTime, int& points, Player::Player& player);
	
	bool ShouldAddPowerUps(int& points);
	bool DidPlayerDied(Player::Player& player);

}

extern Menus gameState;
extern int buttonSelected;