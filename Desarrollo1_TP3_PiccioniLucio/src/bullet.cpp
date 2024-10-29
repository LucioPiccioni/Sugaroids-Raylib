#include "bullet.h"
#include <cmath>
#include <iostream>

Bullet::Bullet Bullet::SpawnBullet(Vector2 playerPos, float playerAngle, PowerUps powerUps, std::list<Sugaroid::Sugaroid>& sugaroids)
{
    float size = 32.0f;
    float speed = 600.0f;

    // Calcula el ángulo inicial basado en el ángulo del jugador
    float playerAngleRadians = playerAngle * (PI / 180.0f);

    if (powerUps.biggerBullets)
        size *= 1.25f;

    if (powerUps.x2BulletSpeed)
        speed *= 1.50f;

    float directionX = cos(playerAngleRadians);
    float directionY = sin(playerAngleRadians);
    Vector2 velocity = { directionX * speed, directionY * speed };

    Bullet newBullet;
    newBullet.position = playerPos;
    newBullet.velocity = velocity;
    newBullet.size = size;
    newBullet.radius = size / 2;
    newBullet.speed = speed;
    newBullet.toDestroy = false;
    newBullet.targetedSugaroid = nullptr;

    // Si la mejora de misiles guiados está activada y hay sugaroids, busca el más cercano
    if (powerUps.guidedMissiles && !sugaroids.empty())
    {
        float closestDistance = std::numeric_limits<float>::max();

        // Encuentra el sugaroid más cercano
        for (Sugaroid::Sugaroid& sugaroid : sugaroids)
        {
            float dx = sugaroid.position.x - playerPos.x;
            float dy = sugaroid.position.y - playerPos.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < closestDistance)
            {
                closestDistance = distance;
                newBullet.targetedSugaroid = &sugaroid;
            }
        }
    }

    return newBullet;
}

void Bullet::Movement(Bullet& bullet, float deltaTime)
{
    // Actualiza la dirección si el misil guiado está activo y tiene un objetivo
    if (bullet.targetedSugaroid != nullptr)
    {
        float angleToSugaroid = atan2f(bullet.targetedSugaroid->position.y - bullet.position.y,
            bullet.targetedSugaroid->position.x - bullet.position.x);
        bullet.velocity = { cosf(angleToSugaroid) * bullet.speed, sinf(angleToSugaroid) * bullet.speed };
    }

    bullet.position.x += bullet.velocity.x * deltaTime;
    bullet.position.y += bullet.velocity.y * deltaTime;
}


