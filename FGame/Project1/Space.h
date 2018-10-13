#ifndef SPACE_H
#define SPACE_H

#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "Planet.h"
#include "cmath"
#include "string"
#include "sstream"
#include "PlanetTextures.h"
#include "EnemySpawner.h"

using namespace std;
using namespace sf;

struct PlanetParams {
public:
	PlanetParams(int type, Color color, Vector2f scale, Vector2f pos);
	PlanetParams();
	int type;
	Color color;
	Vector2f scale;
	Vector2f position;
	float radius;
};

float range(float min, float max);

string getStringSector(int sectorId);
Vector2f loadSpaceSector(String sector);
Vector2f loadSpaceSector(Planet currentPlanet);
bool* getSectorBorders(string sector);

void spawnerUpdate(Vector2f playerPos, float deltaTime);
void drawSpawner(RenderWindow& window, Camera cam, Sprite spawnerSprite);
bool isDied(Vector2f point);

PlanetParams getPlanetParams(int id);

#endif
