#include "Ball.h"

Ball::Ball(float x, float y, float width, float height, D3DXCOLOR color, float velocityX, float velocityY) : RectArk(x, y, width, height, color) {
	Ball::velocityX = velocityX;
	Ball::velocityY = velocityY;
}

bool Ball::isColliding(RectArk* other) {
	return other->magnitudeDiagonal() < (sqrt(pow(other->getX() - x, 2) + pow(other->getY() - y, 2)));
}

void Ball::integrate(float deltaTime) {
	x = x + velocityX * deltaTime;
	y = y + velocityY * deltaTime;
}