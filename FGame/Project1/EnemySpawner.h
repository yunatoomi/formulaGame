#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "SFML/Graphics.hpp"
#include "Enemy.h"

using namespace sf;

Texture getSpawnTexture();

void spawn(Vector2f playerPos);
void setScale(float scale);
void setPosition(Vector2f position);
Enemy getEnemy(int id);
int getEnemyCount();
Vector2f getPosition();
float getScale();
void reset();
void updateEnemy(int id, float deltaTime, Vector2f playerPos);
void eraseEnemy(int id);
bool isEnemyCollided(int id, Vector2f pos);

#endif