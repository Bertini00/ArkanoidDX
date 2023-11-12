#include "Ball.h"

Ball::Ball(float x, float y, float width, float height, D3DXCOLOR color, float velocityX, float velocityY) : RectArk(x, y, width, height, color) {
	Ball::velocityX = velocityX;
	Ball::velocityY = velocityY;
}
Ball::~Ball() {
}

bool Ball::isColliding(RectArk* other) {
	return other->magnitudeDiagonal() < (sqrt(pow(other->getX() - x, 2) + pow(other->getY() - y, 2)));
}

void Ball::integrate(float deltaTime) {
	x = x + velocityX * deltaTime;
	y = y + velocityY * deltaTime;
}

void Ball::horizontalCollision() {
	velocityX *= -1;
}

void Ball::verticalCollision() {
	velocityY *= -1;
}

void Ball::setX(float x) {
	Ball::x = x;
}
void Ball::setY(float y) {
	Ball::y = y;
}

void Ball::setVelocityX(float velocityX) {
	Ball::velocityX = velocityX;
}

void Ball::setVelocityY(float velocityY) {
	Ball::velocityY = velocityY;
}