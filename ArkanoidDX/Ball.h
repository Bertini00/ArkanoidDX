#pragma once

#include "RectArk.h"
#include "Movable.h"

class Ball : public RectArk, Movable
{
public:
	Ball(float x, float y, float width, float height, D3DXCOLOR color, float velocityX, float velocityY);
	~Ball();

	bool isColliding(RectArk* other);

	void integrate(float deltaTime);

	void horizontalCollision();
	void verticalCollision();

	void setX(float x);
	void setY(float y);
	
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);

private:
	float velocityX, velocityY;


};