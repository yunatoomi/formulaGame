#include "PlanetTextures.h"

Texture getPlanet1(double(*f)(int, double), double step) {
	RenderTexture r;
	int blockSize = 10;
	int radius = 25;
	const int realRadius = 25;
	radius *= blockSize;
	int circle[realRadius*2+1][realRadius*2+1];
	for (int i = 0; i <= realRadius * 2; i++) {
		for (int j = 0; j <= realRadius * 2; j++) {
			circle[i][j] = -1;
		}
	}
	if (!r.create((radius+blockSize)*2, (radius+blockSize)*2)) {

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
	for (int i = 0; i <= realRadius * 2; i++) {
		int j = (*f)(i, step) *realRadius*2;
		if (j < 0) continue;
		for (int k = 0; j+k <= realRadius * 2; k += 5) {
			if (circle[j+k][i] == 0) {
				circle[j+k][i] = 1;
			}
		}
	}
	for (int i = 0; i <= realRadius * 2; i++) {
		for (int j = 0; j <= realRadius * 2; j++) {
			if (circle[i][j] == 0) {
				s.setFillColor(Color(120, 120, 120));
				s.setPosition(Vector2f(j*blockSize, i*blockSize));
				r.draw(s);
			}
			else if (circle[i][j] == 1) {
				s.setFillColor(Color(100, 100, 100));
				s.setPosition(Vector2f(j*blockSize, i*blockSize));
				r.draw(s);
			}
		}
	}
	r.display();
	Texture t(r.getTexture());
	return t;
}