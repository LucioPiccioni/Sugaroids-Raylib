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
        Sugaroid::Sugaroid* closestSugaroid = nullptr;
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
                closestSugaroid = &sugaroid;
            }
        }

        // if a target was found it changes trajectory
        if (closestSugaroid != nullptr)
        {
            float angleToSugaroid = atan2(closestSugaroid->position.y - playerPos.y, closestSugaroid->position.x - playerPos.x);
            newBullet.velocity = { cos(angleToSugaroid) * speed, sin(angleToSugaroid) * speed };
        }
    }

    bullets.push_back(newBullet);
}
