#include "bullet.h"
#include <cmath>

void Bullet::SpawnStarBullet(Vector2& playerPos, float& playerAngle,PowerUps& powerUps, std::vector<Bullet>& bullets, std::vector<Sugaroid::Sugaroid>& sugaroids)
{
    float size = 32.0f;
    float speed = 600.0f;

    // Calcula el ángulo inicial basado en el ángulo del jugador
    float playerAngleRadians = playerAngle * (PI / 180.0f);

    // Dirección inicial
    float directionX = cos(playerAngleRadians);
    float directionY = sin(playerAngleRadians);
    Vector2 velocity = { directionX * speed, directionY * speed };

    Bullet newBullet;
    newBullet.position = playerPos;
    newBullet.velocity = velocity;
    newBullet.radius = size / 2;
    newBullet.speed = speed;
    newBullet.toDestroy = false;

    // If Guided Power Up is on and there are sugaroids look for the closest one:
    if (powerUps.guidedMissiles && !sugaroids.empty())
    {
        float closestDistance = std::numeric_limits<float>::max();

        // Find Closest Sugaroid
        for (Sugaroid::Sugaroid& sugaroid : sugaroids)
        {
            float dx = sugaroid.position.x - playerPos.x;
            float dy = sugaroid.position.y - playerPos.y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < closestDistance)
            {
                closestDistance = distance;
                newBullet.targetedSugaroid = &sugaroid;
            }
        }
    }

    bullets.push_back(newBullet);
}

void Bullet::Movement(Bullet& bullet, float& deltaTime)
{
    float angleToSugaroid = 0.0f;

    if (bullet.targetedSugaroid != nullptr)
    {
        switch (bullet.targetedSugaroid->toDestroy)
        {
        case true:

            bullet.targetedSugaroid = nullptr;
            break;

        case false:

            angleToSugaroid = atan2(bullet.targetedSugaroid->position.y - bullet.position.y, bullet.targetedSugaroid->position.x - bullet.position.x);
            bullet.velocity = { cosf(angleToSugaroid) * bullet.speed, sinf(angleToSugaroid) * bullet.speed };
            break;

        default:
            break;
        }
    }

    bullet.position.x += bullet.velocity.x * deltaTime;
    bullet.position.y += bullet.velocity.y * deltaTime;
}
