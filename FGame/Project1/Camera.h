#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Camera {
private:
	Vector2i cameraSize;
	Vector2f cameraPos;
public:
	Camera(Vector2i startSize, Vector2f startPos);
	Vector2i getCameraSize();
	Vector2f getCameraPos();
	void setCameraPos(Vector2f newPos);
};

#endif