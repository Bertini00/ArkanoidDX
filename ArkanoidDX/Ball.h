#pragma once

#include "Rectangle.h"

class Ball : public RectArk
{
public:
	Ball(float x, float y, float width, float height, D3DXCOLOR color, float velocityX, float velocityY);
	~Ball();

	bool isColliding(RectArk* other);

	void integrate(float deltaTime);

private:
	float velocityX, velocityY;

};