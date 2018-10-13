#include "EnemySpawner.h";

const int maxEnemyCount = 10;
const int spawnRate = 5;
const int distanceToSpawn = 1000;
Vector2f position;
float scale = 1;

vector<Enemy> enemiesInfo;

int nextTimeToSpawn = 0;

float vDistance(Vector2f a, Vector2f b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void reset() {
	enemiesInfo.empty();
	nextTimeToSpawn = 0;
}

float calculateAngle(Vector2f playerPos) {
	if (playerPos.y > 0 && playerPos.x > 0) {
		return (atan(playerPos.y / playerPos.x) * 180) / 3.1415926 - 90;
	}
	else if (playerPos.y > 0 && playerPos.x < 0) {
		return (atan(playerPos.y / playerPos.x) * 180) / 3.1415926 + 90;
	}
	else if (playerPos.y < 0 && playerPos.x > 0) {
		return (atan(playerPos.y / playerPos.x) * 180) / 3.1415926 - 90;
	}
	else if (playerPos.y < 0 && playerPos.x < 0) {
		return (atan(playerPos.y / playerPos.x) * 180) / 3.1415926 + 90;
	}
	else if (playerPos.y == 0 && playerPos.x > 0) {
		return -90;
	}
	else if (playerPos.y == 0 && playerPos.x < 0) {
		return 90;
	}
	else if (playerPos.y < 0 && playerPos.x == 0) {
		return 180;
	}
	else if (playerPos.y > 0 && playerPos.x == 0) {
		return 0;
	}
	else {
		return 0;
	}
	
}

bool isEnemyCollided(int id, Vector2f pos) {
	return enemiesInfo[id].isCollision(pos);
}

void eraseEnemy(int id) {
	enemiesInfo.erase(enemiesInfo.begin() + id);
}

void updateEnemy(int id, float deltaTime, Vector2f playerPos) {
	enemiesInfo[id].update(deltaTime, playerPos, position);
}

void spawn(Vector2f playerPos) {
	if (vDistance(playerPos, position) <= distanceToSpawn * scale && time(NULL) > nextTimeToSpawn && enemiesInfo.size() < maxEnemyCount) {
		float angle = calculateAngle(playerPos - position) * 3.1415926 / 180;
		Vector2f pos = position;
		pos.x += cos(angle) * 250 * scale + 50;
		pos.y += sin(angle) * 250 * scale + 50;
		enemiesInfo.push_back(Enemy(pos, 300));
		nextTimeToSpawn = time(NULL) + spawnRate;
	}
}

void setScale(float sc) {
	scale = sc;
}

void setPosition(Vector2f pos) {
	position = pos;
}

Enemy getEnemy(int id) {
	return enemiesInfo[id];
}

int getEnemyCount() {
	return enemiesInfo.size();
}

Texture getSpawnTexture() {
	RenderTexture r;
	int blockSize = 10;
	int radius = 25;
	const int realRadius = 25;
	radius *= blockSize;
	int circle[realRadius * 2 + 1][realRadius * 2 + 1];
	for (int i = 0; i <= realRadius * 2; i++) {
		for (int j = 0; j <= realRadius * 2; j++) {
			circle[i][j] = -1;
		}
	}
	if (!r.create((radius + blockSize) * 2, (radius + blockSize) * 2)) {

	}
	int angleStep = 1;
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
	r.clear(Color(0, 0, 0, 0));
	int previous = 0;
	for (int i = 0; i <= realRadius * 2; i++) {
		int j = realRadius + sin(i) * pow(i, 0.8);
		if (j > previous) {
			for (int k = previous; k < j; k++) {
				if (k > 0 && k < realRadius * 2 && circle[k][i] == 0) {
					circle[k][i] = 1;
				}
			}
		}
		else {
			for (int k = previous; k > j; k--) {
				if (k > 0 && k < realRadius * 2 && circle[k][i] == 0) {
					circle[k][i] = 1;
				}
			}
		}
		previous = j;
	}
	for (int i = 0; i <= realRadius * 2; i++) {
		for (int j = 0; j <= realRadius * 2; j++) {
			if (circle[i][j] == 0) {
				s.setFillColor(Color(20, 20, 120));
				s.setPosition(Vector2f(j*blockSize, i*blockSize));
				r.draw(s);
			}
			else if (circle[i][j] == 1) {
				s.setFillColor(Color(10, 10, 100));
				s.setPosition(Vector2f(j*blockSize, i*blockSize));
				r.draw(s);
			}
		}
	}
	r.display();
	Texture t(r.getTexture());
	return t;
}

Vector2f getPosition() {
	return position;
}

float getScale() {
	return scale;
}