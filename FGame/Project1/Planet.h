#ifndef PLANET_H
#define PLANET_H

#include "SFML/Graphics.hpp"
#include "cmath"
#include "Camera.h"
#include "SupMath.h"
#include "random"
#include "vector"
#include "Tree.h"

using namespace sf;
using namespace std;

const int mapWidth = 250;
const int mapHeight = 80;

class Planet {
private:
	int groundHeight = 5;
	int groundScale = 30;
	int groundLength = 250;
	int groundPadding = 3;

	int genMap[mapHeight][mapWidth];

	RectangleShape block;
	int id;
	mt19937 engine;
	uniform_real_distribution<> dist;
	void generateGround();
	void generateTrees();
	void drawGround(RenderWindow& Window, Camera camera);
	void drawTrees(RenderWindow& Window, Camera camera);
public:
	vector<Tree> trees;

	Planet(int seed);
	void generate();
	void drawPlanet(RenderWindow& Window, Camera camera);
	float getRand();
	int getGroundHeight();
	int getGroundScale();
	int getGroundLength();
	int getGroundPadding();
	vector<Tree> getTrees();
	int getId();
};

#endif