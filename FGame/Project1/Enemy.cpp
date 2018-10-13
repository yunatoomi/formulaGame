#include "Enemy.h"

Sprite enemySprite;

void setEnemyTexture(Texture t) {
	enemySprite.setTexture(t);
}

void test(RenderWindow& window) {
	window.draw(enemySprite);
}

Enemy::Enemy(Vector2f pos, float s) {
	position = pos;
	speed = s;
}

float distanceBetweenVectors(Vector2f a, Vector2f b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
bool Enemy::isCollision(Vector2f collisionPoint) {
	bool r = distanceBetweenVectors(collisionPoint, position) < 100;
	isDied = r;
	return r;
}
float dlerp(float a, float b, float t) {
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
float cAngle(Vector2f r) {
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
	else if (r.y > 0 && r.x == 0) {
		return 0;
	}
	else {
		return 0;
	}
}

void Enemy::update(float deltaTime, Vector2f playerPos, Vector2f spawnPos) {
	if (distanceBetweenVectors(position, playerPos) > radius) {
		angle = dlerp(angle, cAngle(spawnPos - position) + 90, deltaTime * 180);
		position.x += cos(angle*3.1415926 / 180) * speed * deltaTime;
		position.y += sin(angle*3.1415926 / 180) * speed * deltaTime;
	}
	else {
		angle = dlerp(angle, cAngle(playerPos - position) + 60, deltaTime * 180);
		position.x += cos(angle*3.1415926 / 180) * speed * deltaTime;
		position.y += sin(angle*3.1415926 / 180) * speed * deltaTime;
	}
}
void Enemy::draw(RenderWindow& window, Camera cam) {
	enemySprite.setPosition(position - cam.getCameraPos());
	enemySprite.setRotation(angle);
	window.draw(enemySprite);
}

Texture getEnemyTexture() {
	RenderTexture t;
	int enemy[8][10];
	if (!t.create(100, 80)) {

	}
	for (float i = 0; i < 10; i++) {
		float x = (i * 4)/10 + 0.5;
		int y = (pow(pow(x, 3 - x), 1 / x))*2;
		enemy[4 + y][(int)i] = 1;
		enemy[4 - y][(int)i] = 1;
	}
	for (int i = 0; i < 10; i++) {
		bool startedFilling = false;
		for (int j = 0; j < 8; j++) {
			if (startedFilling && enemy[j][i] == 1 && j >= 4) {
				startedFilling = false;
			}
			else if (!startedFilling && enemy[j][i] == 1 && j < 4) {
				startedFilling = true;
			}
			else if (startedFilling) {
				enemy[j][i] = 1;
			}
		}
	}
	for (float i = 2; i < 9; i++) {
		float x = ((i-2) * 4) / 6 + 0.5;
		int y = (pow(pow(x, 3 - x), 1 / x));
		enemy[4 + y][(int)i] = 0;
		enemy[4 - y][(int)i] = 0;
	}
	for (int i = 0; i < 10; i++) {
		bool startedFilling = false;
		for (int j = 0; j < 8; j++) {
			if (startedFilling && enemy[j][i] == 0 && j >= 4) {
				startedFilling = false;
			}
			else if (!startedFilling && enemy[j][i] == 0 && j < 4) {
				startedFilling = true;
			}
			else if (startedFilling) {
				enemy[j][i] = 0;
			}
		}
	}
	RectangleShape r(Vector2f(10, 10));
	t.clear(Color(0, 0, 0, 0));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10; j++) {
			if (enemy[i][j] == 1) {
				r.setFillColor(Color(120, 50, 50));
				r.setPosition(Vector2f(j * 10, i * 10));
				t.draw(r);
			}
			else if (enemy[i][j] == 0) {
				r.setFillColor(Color(80, 50, 50));
				r.setPosition(Vector2f(j * 10, i * 10));
				t.draw(r);
			}
		}
	}
	t.display();
	return t.getTexture();
}