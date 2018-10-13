#include "Bullet.h"

Sprite bullet1;

void setBulletTexture(Texture t) {
	bullet1.setTexture(t);
}

void Bullet::print() {
	cout << "Bullet{" << position.x << "," << position.y << ";" << angle << ";" << direction.x << "," << direction.y << "}" << endl;
}

Sprite getBulletSprite() {
	return bullet1;
}

Bullet::Bullet(Vector2f pos, float a, float speed) {
	position = pos;
	Vector2f dir(0, 0);
	angle = a;
	dir.y = sin(angle * 3.1415926 / 180) * speed;
	dir.x = cos(angle * 3.1415926 / 180) * speed;
	direction = dir;
}

void Bullet::update(float deltaTime) {
	Vector2f acc = direction;
	acc.x *= deltaTime;
	acc.y *= deltaTime;
	position = position + acc;
}

bool Bullet::isDie() {
	if (position.x < 0 || position.x > 6000 || position.y < 0 || position.y > 6000)
		return true;
	return false;
}

void Bullet::draw(RenderWindow& window, Camera cam) {
	bullet1.setPosition(position - cam.getCameraPos());
	bullet1.setRotation(angle);
	window.draw(bullet1);
}

Vector2f Bullet::getPosition() {
	return position;
}

Texture getBulletTexture() {
	RenderTexture r;
	if (!r.create(30, 30)) {
		
	}
	RectangleShape s(Vector2f(5, 5));
	s.setFillColor(Color(155, 155, 20));
	int bull[6][6];
	for (int i = 0; i < 6; i++) {
		int y = floor(sqrt(i));
		bull[y][i] = 1;
		bull[5 - y][i] = 1;
	}
	for (int i = 0; i < 6; i++) {
		bool startedFilling = false;
		for (int j = 0; j < 6; j++) {
			if (startedFilling && bull[j][i] == 1 && j >= 3) {
				startedFilling = false;
			}
			else if (!startedFilling && bull[j][i] == 1 && j < 3) {
				startedFilling = true;
			}
			else if (startedFilling) {
				bull[j][i] = 1;
			}
		}
	}
	r.clear(Color(0, 0, 0, 0));
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (bull[i][j] == 1) {
				s.setPosition(Vector2f(j * 5, i * 5));
				r.draw(s);
			}
		}
	}
	r.display();
	return r.getTexture();
}