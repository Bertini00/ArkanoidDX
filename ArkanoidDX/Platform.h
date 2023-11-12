#pragma once

#include "RectArk.h"
#include "Movable.h"


class Platform : public RectArk, Movable
{
public:
	Platform(float x, float y, float width, float height, D3DXCOLOR color, float velocityX);
	~Platform();

	void changeDirection(int dir);

	void integrate(float deltaTime);

private:
	float velocityX;
	int direction;

};
