#include "player.h"
#include <cmath>

void Player::Movement(Player& player, float deltaTime, int screenWidth, int screenHeight)
{
	const float acceleration = 600.0f; // Aceleración al moverse
	const float minSpeed = 20.0f; // Velocidad mínima para continuar moviéndose

	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		player.targetPos = GetMousePosition();

		// Calculate direction of player to mouse
		player.directionVector = { player.targetPos.x - player.pos.x, player.targetPos.y - player.pos.y };
		player.directionLength = sqrt(player.directionVector.x * player.directionVector.x + player.directionVector.y * player.directionVector.y);

		// Normalize direction
		if (player.directionLength > 0)
		{
			player.directionVector.x /= player.directionLength;
			player.directionVector.y /= player.directionLength;

			// Acelera el jugador hacia la dirección del objetivo
			player.speed.x += player.directionVector.x * acceleration * deltaTime;
			player.speed.y += player.directionVector.y * acceleration * deltaTime;
		}
	}
	else
	{
		/*
		float currentSpeed = sqrt(player.speed.x * player.speed.x + player.speed.y * player.speed.y);
		if (currentSpeed < minSpeed)
		{
			player.speed.x = 0;
			player.speed.y = 0;
		}
		*/
	}

	// Limit Max Velocity
	float currentSpeed = sqrt(player.speed.x * player.speed.x + player.speed.y * player.speed.y);
	if (currentSpeed > player.maxSpeed)
	{
		player.speed.x = (player.speed.x / currentSpeed) * player.maxSpeed;
		player.speed.y = (player.speed.y / currentSpeed) * player.maxSpeed;
	}

	player.pos.x += player.speed.x * deltaTime;
	player.pos.y += player.speed.y * deltaTime;

	if (player.pos.x < 0) player.pos.x = screenWidth;
	if (player.pos.x > screenWidth) player.pos.x = 0;
	if (player.pos.y < 0) player.pos.y = screenHeight;
	if (player.pos.y > screenHeight) player.pos.y = 0;
}

void Player::Shoot(Player& player, Sound& shootSound, std::vector<Bullet::Bullet>& bullets)
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		StopSound(shootSound);
		PlaySound(shootSound);
		Bullet::SpawnStarBullet(player.pos, player.angle, bullets);
	}
}
