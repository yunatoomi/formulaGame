#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "iostream"

using namespace sf;
using namespace std;

Texture getBulletTexture();
void setBulletTexture(Texture t);
Sprite getBulletSprite();

class Bullet {
private:
	Vector2f position;
	Vector2f direction;
	float angle;
public:
	void print();
	Bullet(Vector2f position, float angle, float speed);
	void update(float deltaTime);
	void draw(RenderWindow& window, Camera cam);
	bool isDie();
};

#endif
