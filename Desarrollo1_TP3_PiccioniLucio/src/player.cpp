#include "player.h"
#include <cmath>
#include "bullet.h"

void Player::Movement(Player& player, float deltaTime, int screenWidth, int screenHeight)
{
	const float acceleration = 600.0f;

	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		player.targetPos = GetMousePosition();

		// Calcular dirección del jugador hacia el mouse
		player.directionVector = { player.targetPos.x - player.pos.x, player.targetPos.y - player.pos.y };
		player.directionLength = sqrtf(player.directionVector.x * player.directionVector.x + player.directionVector.y * player.directionVector.y);

		// Normalizar la dirección
		if (player.directionLength > 0)
		{
			// Resistencia y movimiento curvo
			player.directionVector.x /= player.directionLength;
			player.directionVector.y /= player.directionLength;

			// Acelerar el jugador hacia la dirección del objetivo
			player.speed.x += player.directionVector.x * acceleration * deltaTime;
			player.speed.y += player.directionVector.y * acceleration * deltaTime;
		}
	}

	// Limitar velocidad máxima
	float currentSpeed = sqrt(player.speed.x * player.speed.x + player.speed.y * player.speed.y);
	if (currentSpeed > player.maxSpeed)
	{
		player.speed.x = (player.speed.x / currentSpeed) * player.maxSpeed;
		player.speed.y = (player.speed.y / currentSpeed) * player.maxSpeed;
	}

	player.pos.x += player.speed.x * deltaTime;
	player.pos.y += player.speed.y * deltaTime;

	// Teletransporte si el jugador sale de la pantalla
	if (player.pos.x < 0) player.pos.x = static_cast<float>(screenWidth);
	if (player.pos.x > screenWidth) player.pos.x = 0;
	if (player.pos.y < 0) player.pos.y = static_cast<float>(screenHeight);
	if (player.pos.y > screenHeight) player.pos.y = 0;
}

void Player::Shoot(Player& player, Sound& shootSound, std::list<Bullet::Bullet>& bullets, std::list<Sugaroid::Sugaroid>& sugaroids)
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		StopSound(shootSound);
		PlaySound(shootSound);

		if (player.boost.x3Bullets)
			player.bulletBuffer = 3;
		else
			player.bulletBuffer = 1;
	}

	if (player.fireCooldown <= 0)
	{
		if (player.bulletBuffer > 0)
		{
			Bullet::SpawnStarBullet(player.pos, player.angle, player.boost, bullets, sugaroids);

			player.bulletBuffer--;
			player.fireCooldown = 1;
		}
	}
	else
	{
		player.fireCooldown -= 0.5;
	}
}
