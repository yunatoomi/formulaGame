#include "Planet.h"

const int bSize = 10;

const double M_PI = 3.14;

void Planet::generate() {
	generateGround();
	generateTrees();
}

void Planet::generateGround() {
	for (int i = 0; i < groundLength; i++) {
		int groundC = ceil(abs(sin(((i + 1)*groundScale*M_PI)))*groundHeight);
		for (int j = 1; j <= groundPadding + 1; j++) {
			genMap[mapHeight - j][i] = 1;
		}
		for (int j = groundHeight; j > 0; j--) {
			if (j < groundC) {
				genMap[mapHeight - j - groundPadding - 1][i] = 1;
			}
		}
	}
}

void Planet::generateTrees() {
	Vector2f distanceRange(100, 500);
	for (float i = distanceRange.x + getRand()*(distanceRange.y - distanceRange.x); i < mapWidth*bSize; i += distanceRange.x + getRand()*(distanceRange.y - distanceRange.x)) {
		for (int j = groundPadding; j <= groundPadding + groundHeight; j++) {
			if (genMap[mapHeight - j ][(int)floor(i / bSize)] == 0) {
				Tree newTree(Branch(Vector2f(i, (mapHeight-j + 2)*bSize), 8, 60, 180, 3), getRand()*123456, 3);
				trees.push_back(newTree);
				break;
			}
		}
	}
}

Planet::Planet(int seed) {
	Vector2i groundHeightRange(3, 7);
	Vector2i groundScaleRange(20, 40);
	Vector2i groundPaddingRange(2, 7);
	id = seed;
	engine = mt19937(seed);
	groundHeight = groundHeightRange.x + getRand() * (groundHeightRange.y - groundHeightRange.x);
	groundScale = groundScaleRange.x + getRand() * (groundScaleRange.y - groundScaleRange.x);
	groundPadding = groundPaddingRange.x + getRand() * (groundPaddingRange.y - groundPaddingRange.x);
	
	block.setSize(Vector2f(bSize, bSize));
	block.setFillColor(Color(0, 0, 0));

	generate();
}

int Planet::getId() {
	return id;
}

void Planet::drawPlanet(RenderWindow& Window, Camera camera) {
	drawTrees(Window, camera);
	drawGround(Window, camera);
}

vector<Tree> Planet::getTrees() {
	return trees;
}

void Planet::drawTrees(RenderWindow& Window, Camera camera) {
	for (int i = 0; i < trees.size(); i++) {
		Vector2f v = trees[i].getPos();
		if (v.x - 100 < camera.getCameraPos().x + camera.getCameraSize().x && v.x + 100 > camera.getCameraPos().x) {
			trees[i].draw(Window, camera);
		}
	}
}

void Planet::drawGround(RenderWindow& Window, Camera camera) {
	Vector2f cameraPos = camera.getCameraPos();
	Vector2i cameraSize = camera.getCameraSize();
	for (int i = clamp(0, mapHeight, floor(cameraPos.y / bSize)); i < clamp(0, mapHeight, ceil((cameraPos.y + cameraSize.y) / bSize)); i++) {
		for (int j = clamp(0, mapWidth, floor(cameraPos.x / bSize)); j < clamp(0, mapWidth, ceil((cameraPos.x + cameraSize.x) / bSize)); j++) {
			if (genMap[i][j] == 1) {
				block.setPosition(Vector2f(-cameraPos.x + j*bSize, -cameraPos.y + i*bSize));
				Window.draw(block);
			}
		}
	}
}

float Planet::getRand() {
	return dist(engine);
}

int Planet::getGroundHeight() {
	return groundHeight;
}
int Planet::getGroundScale() {
	return groundScale;
}
int Planet::getGroundLength() {
	return groundLength;
}
int Planet::getGroundPadding() {
	return groundPadding;
}

