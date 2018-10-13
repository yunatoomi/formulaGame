#include "Player.h"

const Vector2i playerTextureSize(240, 100);
const int playerQuality = 10;
vector<Bullet> bullets;

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

float lerp(float a, float b, float t) {
	if (a > b) {
		a -= t;
		if (a < b) {
			return b;
		}
	}
	else if (a < b) {
		a += t;
		if (a > b) {
			return b;
		}
	}
	return a;
}

void Player::fire(RenderWindow& window, Camera cam, float deltaTime, bool(*f)(Vector2f)) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].update(deltaTime);
		bullets[i].draw(window, cam);
		if (f(bullets[i].getPosition())) {
			bullets.erase(bullets.begin() + i);
		}
		else {
			if (bullets[i].isDie())
				bullets.erase(bullets.begin() + i);
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Space) && time(NULL) > nextTimeToShoot) {
		Bullet bullet(getPosition(), currentAngle+90, getMaxSpeed().x * 3);
		bullets.push_back(bullet);
		nextTimeToShoot = time(NULL) + shootSpeed;
	}
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

void Player::setPlayerSpaceSpriteTexture(Texture texture) {
	spaceSprite.setTexture(texture);
	spaceSprite.setOrigin(Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
}

void Player::setPosition(Vector2f pos) {
	position = pos;
}

void Player::draw(RenderWindow& Window, Camera camera) {
	sprite.setPosition(getPosition() - camera.getCameraPos());
	sprite.setRotation(getCurrentAngle());
	Window.draw(sprite);
}

void Player::spaceDraw(RenderWindow& Window, Camera camera, float deltaTime, bool (*f)(Vector2f)) {
	spaceSprite.setScale(Vector2f(0.5f, 0.5f));
	spaceSprite.setPosition(getPosition() - camera.getCameraPos());
	spaceSprite.setRotation(getCurrentAngle());
	fire(Window, camera, deltaTime, *f);
	Window.draw(spaceSprite);
}

Texture generateSpacePlayerTexture() {
	RenderTexture r;
	int blockSize = 10;
	int radius = 10;
	const int realRadius = 10;
	int secondRadius = 5;
	radius *= blockSize;
	secondRadius *= blockSize;
	int circle[realRadius * 2 + 1][realRadius * 2 + 1];
	for (int i = 0; i <= realRadius * 2; i++) {
		for (int j = 0; j <= realRadius * 2; j++) {
			circle[i][j] = -1;
		}
	}
	if (!r.create((radius + blockSize) * 2, (radius + blockSize) * 2)) {

	}
	int angleStep = 7;
	for (int i = 0; i <= 360; i += angleStep) {
		int x = ceil((cos(i*3.1415926 / 180) * radius) / blockSize);
		int y = ceil((sin(i*3.1415926 / 180) * radius) / blockSize);
		circle[realRadius + x][realRadius + y] = 1;
		circle[realRadius + x][realRadius - y] = 1;
		circle[realRadius - x][realRadius + y] = 1;
		circle[realRadius - x][realRadius - y] = 1;
	}
	for (int i = 0; i <= realRadius * 2; i++) {
		bool startedFilling = false;
		for (int j = 0; j <= realRadius * 2; j++) {
			if (startedFilling && circle[i][j] == 1 && j >= realRadius) {
				startedFilling = false;
			}
			else if (!startedFilling && circle[i][j] == 1 && j < realRadius) {
				startedFilling = true;
			}
			else if (startedFilling && circle[i][j] == -1) {
				circle[i][j] = 0;
			}
		}
	}
	RectangleShape s(Vector2f(blockSize, blockSize));
	int previous = 0;
	for (double i = 0; i <= realRadius * 2 - 8; i++) {
		int x = round(pow(pow(i, 7 - i), 0.8 / i));
		circle[(int)i + 4][realRadius + x] = 1;
		circle[(int)i + 4][realRadius - x] = 1;

		for (int j = previous; j < x; j++) {
			circle[(int)i + 4][realRadius + j] = 1;
			circle[(int)i + 4][realRadius - j] = 1;
		}

		previous = x;
	}
	for (int i = 0; i <= realRadius * 2; i++) {
		for (int j = 0; j <= realRadius * 2; j++) {
			if (circle[i][j] == 0) {
				s.setFillColor(Color(255, 255, 255));
				s.setPosition(Vector2f(j*blockSize, i*blockSize));
				r.draw(s);
			}
			else if (circle[i][j] == 1) {
				s.setFillColor(Color(0, 0, 0));
				s.setPosition(Vector2f(j*blockSize, i*blockSize));
				r.draw(s);
			}
		}
	}
	r.display();
	return r.getTexture();
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


void Player::movement(float deltaTime, Vector2f mapSizes, float minHeight) {
	if (time(NULL) >= freezedTo) {
		Vector2f acc = calculateAcceleration(deltaTime);
		bool* isStopping = accelerationToStopping(acc);
		normalizeSpeed(mapSizes, minHeight);
		currentAngle = calculateAngle();
		stoppingMovement(isStopping[0], isStopping[1], deltaTime);
	}
	moveBySpeed(deltaTime);
}

void Player::spaceMovement(float deltaTime, Vector2f minSpaceBarriers, Vector2f maxSpaceBarriers) {
	if (time(NULL) >= freezedTo) {
		Vector2f acc = calculateAcceleration(deltaTime);
		acc.x *= 0.5f;
		acc.y *= 0.5f;
		bool* isStopping = accelerationToStopping(acc);
		normalizeSpeed(minSpaceBarriers, maxSpaceBarriers);
		currentAngle = lerp(currentAngle, calculateSpaceAngle(isStopping, acc), deltaTime*180);
		stoppingMovement(isStopping[0], isStopping[1], deltaTime);
	}
	moveBySpeed(deltaTime);
}

float Player::calculateSpaceAngle(bool* stopping, Vector2f acc) {
	if (!stopping[0]) {
		acc.x = 0;
	}
	if (!stopping[1]) {
		acc.y = 0;
	}
	Vector2f r;
	if (stopping[0] || stopping[1]) {
		r = acc;
	}
	else {
		r = speed;
	}
	if (r.y > 0 && r.x > 0) {
		return (atan(r.y / r.x) * 180) / 3.1415926 - 90;
	}
	else if (r.y > 0 && r.x < 0) {
		return (atan(r.y / r.x) * 180) / 3.1415926 + 90;
	}
	else if (r.y < 0 && r.x > 0) {
		return (atan(r.y / r.x) * 180) / 3.1415926 - 90;
	}
	else if (r.y < 0 && r.x < 0) {
		return (atan(r.y / r.x) * 180) / 3.1415926 + 90;
	}
	else if (r.y == 0 && r.x > 0) {
		return -90;
	}
	else if (r.y == 0 && r.x < 0) {
		return 90;
	}
	else if (r.y < 0 && r.x == 0) {
		return 180;
	}
	else if (r.y > 0 && r.x == 0){
		return 0;
	}
	else {
		return currentAngle;
	}
}

void Player::setSpeed(Vector2f sp) {
	speed = sp;
}

float Player::calculateAngle() {
	return (speed.x / maxSpeed.x) * maxAngle;
}

void Player::moveBySpeed(float deltaTime) {
	position.y += speed.y * deltaTime;
	position.x += speed.x * deltaTime;
}

void Player::normalizeSpeed(Vector2f mapSize, float minHeight) {
	Vector2f xBarrier(pClamp(-maxSpeed.x, 0, ((position.x-(playerTextureSize.x/2)) * -maxSpeed.x)/500 ), pClamp(0, maxSpeed.x, ((mapSize.y - position.x-50-(playerTextureSize.x/2))*maxSpeed.x)/500 ));

	speed.x = pClamp(xBarrier.x, xBarrier.y, speed.x);
	speed.y = pClamp(-maxSpeed.y, pClamp(0, maxSpeed.y, ((minHeight-position.y-(playerTextureSize.y /2))*maxSpeed.y)/100), speed.y);
}

void Player::normalizeSpeed(Vector2f minSpaceBarriers, Vector2f maxSpaceBarriers) {
	Vector2f xBarrier(pClamp(-maxSpeed.x * 0.5f, 0, ((position.x - (spaceSprite.getLocalBounds().width / 2)) * -maxSpeed.x * 0.5f) / 500), pClamp(0, maxSpeed.x*0.5f, ((maxSpaceBarriers.x - position.x - 50 - (spaceSprite.getLocalBounds().width / 2))*maxSpeed.x*0.5f) / 500));
	if (minSpaceBarriers.x == -1) {
		xBarrier.x = -maxSpeed.x;
	}
	if (maxSpaceBarriers.x == -1) {
		xBarrier.y = maxSpeed.x;
	}
	Vector2f yBarrier(pClamp(-maxSpeed.y * 0.5f, 0, ((position.y - (spaceSprite.getLocalBounds().height / 2)) * -maxSpeed.y * 0.5f) / 500), pClamp(0, maxSpeed.y*0.5f, ((maxSpaceBarriers.y - position.y - 50 - (spaceSprite.getLocalBounds().height / 2))*maxSpeed.y*0.5f) / 500));
	if (minSpaceBarriers.y == -1) {
		yBarrier.x = -maxSpeed.y;
	}
	if (maxSpaceBarriers.y == -1) {
		yBarrier.y = maxSpeed.y;
	}
	speed.x = pClamp(xBarrier.x, xBarrier.y, speed.x);
	speed.y = pClamp(yBarrier.x, yBarrier.y, speed.y);
}

bool* Player::accelerationToStopping(Vector2f acceleration) {
	bool* retArr = new bool[2];
	if (acceleration.x == 0) {
		retArr[0] = false;
	}
	else {
		retArr[0] = true;
	}
	if (acceleration.y == 0) {
		retArr[1] = false;
	}
	else {
		retArr[1] = true;
	}
	return retArr;
}

Vector2f Player::calculateAcceleration(float deltaTime) {
	Vector2f acc(0,0);
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		acc.y -= acceleration * deltaTime;
	};
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		acc.x -= acceleration * deltaTime;
	};
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		acc.y += acceleration * deltaTime;
	};
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		acc.x += acceleration * deltaTime;
	};
	speed = speed + acc;
	return acc;
}

Vector2f Player::getMaxSpeed() {
	return maxSpeed;
}

void Player::stoppingMovement(bool changedX, bool changedY, float deltaTime) {
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

void Player::planetUpdate(float deltaTime, Vector2f mapSizes, float minHeight) {
	movement(deltaTime, mapSizes, minHeight);
}

void Player::spaceUpdate(float deltaTime, Vector2f minSpaceBarriers, Vector2f maxSpaceBarriers) {
	spaceMovement(deltaTime, minSpaceBarriers, maxSpaceBarriers);
}

void Player::freeze(int t) {
	freezedTo = time(NULL) + t;
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
