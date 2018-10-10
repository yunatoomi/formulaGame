#include "Player.h"

const Vector2i playerTextureSize(240, 100);
const int playerQuality = 10;

float pClamp(float min, float max, float val) {
	if (val < min) {
		return min;
	}
	else if (val > max) {
		return max;
	}
	return val;
}

double normalizeF(double val, double cellVal) {
	return floor(val / cellVal) * cellVal;
}

double normalizeC(double val, double cellVal) {
	return ceil(val / cellVal) * cellVal;
}

double normalizeR(double val, double cellVal) {
	return round(val / cellVal) * cellVal;
}

double normalizeT(double val, double cellVal) {
	return trunc(val / cellVal) * cellVal;
}

Player::Player(Vector2f startPosition, Vector2f mxSp, Vector2f startOffset, float acc, float angl) {
	acceleration = acc;
	maxSpeed = mxSp;
	maxAngle = angl;
	position = Vector2f(startPosition.x, startPosition.y);
	offset = Vector2f(startOffset.x, startOffset.y);
}

void Player::setPlayerSpriteTexture(Texture texture) {
	sprite.setTexture(texture);
	sprite.setOrigin(Vector2f(playerTextureSize.x / 2, playerTextureSize.y / 2));
}

void Player::draw(RenderWindow& Window, Camera camera) {
	sprite.setPosition(getPosition() - camera.getCameraPos());
	sprite.setRotation(getCurrentAngle());
	Window.draw(sprite);
}

Texture generatePlayerTexture() {

	RenderTexture playerRenderer;
	if (!playerRenderer.create(playerTextureSize.x, playerTextureSize.y)) {
		cout << "not working" << endl;
	}
	RectangleShape block(Vector2f(playerQuality, playerQuality));
	block.setFillColor(Color(0, 0, 0));
	block.setPosition(10, 10);

	playerRenderer.clear(Color(0, 0, 0, 0));
	float step = 0.05f;
	float maxVal = 1.35f;
	int times = floor(maxVal / step);
	double previousX = 0;
	double previousX2 = 0;
	for (int i = 0; i < times; i++) {
		double x = step*i;
		double y = pClamp(0, 9999999, pow((pow(x, 1 / 2.8f) + pow(x, 10.9f - pow(x, 9.3 - x))), 0.5) - 0.3);
		if (y != 0) {
			double realX = normalizeR(playerTextureSize.x / 2 + 0.3 + (y / 2)*playerTextureSize.x / 2, playerQuality);
			double realY = normalizeC((x / 1.6)*playerTextureSize.y, playerQuality);
			if (realY == 5 * playerQuality) {
				block.setSize(Vector2f(realX - normalizeT(playerTextureSize.x / 2 + 0.3 + ((y * -1 + 0.6) / 2)*playerTextureSize.x / 2, playerQuality), playerQuality));
				block.setPosition(Vector2f(normalizeT(playerTextureSize.x / 2 + 0.3 + ((y * -1 + 0.6) / 2)*playerTextureSize.x / 2, playerQuality), realY));
				playerRenderer.draw(block);
				block.setSize(Vector2f(playerQuality, playerQuality));
			}
			previousX = realX;
			block.setPosition(Vector2f(realX, realY));
			playerRenderer.draw(block);
			y = y * -1 + 0.6;
			realX = normalizeT(playerTextureSize.x / 2 + 0.3 + (y / 2)*playerTextureSize.x / 2, playerQuality);
			block.setPosition(Vector2f(realX, realY));
			previousX2 = realX;
			playerRenderer.draw(block);
		}
	}
	block.setSize(Vector2f(previousX - previousX2, playerQuality));
	playerRenderer.draw(block);
	playerRenderer.display();
	return playerRenderer.getTexture();
}

void Player::update(float deltaTime) {
	position.y += speed.y * deltaTime;
	position.x += speed.x * deltaTime;
	bool changedX = false;
	bool changedY = false;
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		speed.y -= acceleration * deltaTime;
		changedY = true;
	};
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		speed.x -= acceleration * deltaTime;
		changedX = true;
	};
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		speed.y += acceleration * deltaTime;
		changedY = true;
	};
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		speed.x += acceleration * deltaTime;
		changedX = true;
	};
	speed.x = pClamp(-maxSpeed.x, maxSpeed.x, speed.x);
	speed.y = pClamp(-maxSpeed.y, maxSpeed.y, speed.y);
	currentAngle = (speed.x / maxSpeed.x) * maxAngle;
	if (!changedX) {
		if (speed.x > 0.1) {
			speed.x -= acceleration * deltaTime;
		}
		else if (speed.x < -0.1) {
			speed.x += acceleration * deltaTime;
		}
		else {
			speed.x = 0;
		}
	}
	if (!changedY) {
		if (speed.y > 0.1) {
			speed.y -= acceleration * deltaTime;
		}
		else if (speed.y < -0.1) {
			speed.y += acceleration * deltaTime;
		}
		else {
			speed.y = 0;
		}
	}
}

Vector2f Player::getPosition() {
	return position;
}

Vector2f Player::getOffset() {
	return offset;
}

float Player::getCurrentAngle() {
	return currentAngle;
}
