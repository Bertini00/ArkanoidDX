#include "Platform.h"
#include <algorithm>

Platform::Platform(float x, float y, float width, float height, D3DXCOLOR color, float velocityX) : RectArk(x, y, width, height, color) {
	Platform::velocityX = velocityX;
	direction = 0.0f;
}

Platform::~Platform(){

}

void Platform::changeDirection(int dir) {
	direction += dir;
	direction = std::clamp(direction, -1, 1);
}

void Platform::integrate(float deltaTime) {
	x = x + velocityX * deltaTime * direction;
	direction = 0;
}