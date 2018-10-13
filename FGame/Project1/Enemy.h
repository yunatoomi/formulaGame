#ifndef ENEMY_H
#define ENEMY_H

#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "iostream"

using namespace sf;
using namespace std;

Texture getEnemyTexture();

void setEnemyTexture(Texture t);
void test(RenderWindow& window);

class Enemy {
private:
	Vector2f position;
	float speed;
	float angle = 0;
	float radius = 500;
public:
	bool isDied = false;
	Enemy(Vector2f pos, float speed);
	bool isCollision(Vector2f collisionPoint);
	void update(float deltaTime, Vector2f playerPos, Vector2f spawnPos);
	void draw(RenderWindow& window, Camera cam);
};

#endif
