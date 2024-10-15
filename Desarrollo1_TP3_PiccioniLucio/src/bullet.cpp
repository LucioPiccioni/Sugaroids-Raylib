#include "bullet.h"

void Bullet::SpawnStarBullet(Vector2 playerPosition, float playerAngle, std::vector<Bullet>& bullets)
{
	float size = 32.0f;
	float speed = 500.0f;

	float playerAngleRadians = playerAngle * (PI / 180.0f);

	float directionX = cos(playerAngleRadians);
	float directionY = sin(playerAngleRadians);

	Vector2 velocity = { directionX * speed, directionY * speed };

	Bullet newBullet;
	newBullet.position = playerPosition;
	newBullet.velocity = velocity;
	newBullet.radius = size / 2;
	newBullet.speed = speed;
	newBullet.toDestroy = false;

	bullets.push_back(newBullet);
}