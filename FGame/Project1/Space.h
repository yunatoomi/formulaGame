#ifndef SPACE_H
#define SPACE_H

#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "Planet.h"

using namespace sf;

Sprite planet1();
Sprite planet2();
Sprite planet3();

Vector2f loadSpaceSector(String sector);

void drawSpace(RenderWindow& window, Camera cam);

#endif
