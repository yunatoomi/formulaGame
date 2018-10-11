#ifndef PLANET_TEXTURES_H
#define PLANET_TEXTURES_H

#include "SFML/Graphics.hpp"
#include "cmath"
#include "iostream"

using namespace sf;
using namespace std;

Texture getPlanet1(double(*f)(int, double), double step);

#endif