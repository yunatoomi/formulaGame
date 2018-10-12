#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "iostream"
#include "time.h"
#include "Bullet.h"

using namespace std;
using namespace sf;

Texture generatePlayerTexture();
Texture generateSpacePlayerTexture();

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
	Sprite spaceSprite;
	int freezedTo = 0;
	int nextTimeToShoot = 0;
	int shootSpeed = 1;
	void movement(float deltaTime, Vector2f mapSizes, float minHeight);
	void stoppingMovement(bool changedX, bool changedY, float deltaTime);
	Vector2f calculateAcceleration(float deltaTime);
	bool* accelerationToStopping(Vector2f acceleration);
	void moveBySpeed(float deltaTime);
	float calculateAngle();
	float calculateSpaceAngle(bool* stopping, Vector2f acc);
	void spaceMovement(float deltaTime, Vector2f minSpaceBarriers, Vector2f maxSpaceBarriers);
	void normalizeSpeed(Vector2f minSpaceBarriers, Vector2f maxSpaceBarriers);
	void normalizeSpeed(Vector2f mapSizes, float minHeight);
public:
	Player(Vector2f startPosition, Vector2f startOffset, Vector2f mxSp, float acc, float angl);

	void planetUpdate(float deltaTime, Vector2f mapBorders, float minHeight);
	void spaceUpdate(float deltaTime, Vector2f minSpaceBarriers, Vector2f maxSpaceBarriers);

	Vector2f getPosition();
	Vector2f getOffset();
	float getCurrentAngle();
	void freeze(int time);
	void draw(RenderWindow& Window, Camera camera);
	void spaceDraw(RenderWindow& Window, Camera camera, float deltaTime);
	void setPlayerSpriteTexture(Texture texture);
	void setPlayerSpaceSpriteTexture(Texture texture);
	void setPosition(Vector2f pos);
	void setSpeed(Vector2f speed);
	Vector2f getMaxSpeed();
	void fire(RenderWindow& window, Camera cam, float deltaTime);
};

#endif