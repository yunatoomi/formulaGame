#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "iostream"

using namespace std;
using namespace sf;

Texture generatePlayerTexture();

class Player {
private:
	Vector2f position;
	Vector2f offset;
	Vector2f speed;
	Vector2f maxSpeed;
	float acceleration;
	float maxAngle;
	float currentAngle;
	Sprite sprite;
public:
	Player(Vector2f startPosition, Vector2f startOffset, Vector2f mxSp, float acc, float angl);

	void update(float deltaTime);
	
	Vector2f getPosition();
	Vector2f getOffset();
	float getCurrentAngle();
	void draw(RenderWindow& Window, Camera camera);
	void setPlayerSpriteTexture(Texture texture);
};

#endif