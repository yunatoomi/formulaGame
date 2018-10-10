#ifndef SUPMATH_H
#define SUPMATH_H

#include <cmath>

float clamp(float min, float max, float val);

float normalizeF(float val, float cellVal);
float normalizeC(float val, float cellVal);
float normalizeR(float val, float cellVal);
float normalizeT(float val, float cellVal);

#endif
