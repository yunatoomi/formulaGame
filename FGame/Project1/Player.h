#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "iostream"
#include "time.h"

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
	Sprite upSprite;
	int freezedTo = 0;
	void movement(float deltaTime, Vector2f mapSizes, float minHeight);
	void stoppingMovement(bool changedX, bool changedY, float deltaTime);
	bool* calculateAcceleration(float deltaTime);
	void moveBySpeed(float deltaTime);
	void normalizeSpeed(Vector2f mapSizes, float minHeight);
public:
	Player(Vector2f startPosition, Vector2f startOffset, Vector2f mxSp, float acc, float angl);

	void planetUpdate(float deltaTime, Vector2f mapSizes, float minHeight);
	void spaceUpdate(float deltaTime);

	Vector2f getPosition();
	Vector2f getOffset();
	float getCurrentAngle();
	void freeze(float time);
	void draw(RenderWindow& Window, Camera camera);
	void setPlayerSpriteTexture(Texture texture);
	void setPlayerUpSpriteTexture(Texture texture);
};

#endif