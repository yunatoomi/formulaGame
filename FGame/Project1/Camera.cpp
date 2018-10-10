#include "Camera.h"

Vector2i Camera::getCameraSize() {
	return cameraSize;
}

Vector2f Camera::getCameraPos() {
	return cameraPos;
}

Camera::Camera(Vector2i startSize, Vector2f startPos) {
	cameraSize = Vector2i(startSize.x, startSize.y);
	cameraPos = Vector2f(startPos.x, startPos.y);
}

void Camera::setCameraPos(Vector2f newPos) {
	cameraPos = newPos;
}