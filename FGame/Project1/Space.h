#ifndef SPACE_H
#define SPACE_H

#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "Planet.h"
#include "cmath"
#include "string"
#include "sstream"
#include "PlanetTextures.h"

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
};

float range(float min, float max);

Vector2f loadSpaceSector(String sector);
Vector2f loadSpaceSector(Planet currentPlanet);

PlanetParams getPlanetParams(int id);

#endif
