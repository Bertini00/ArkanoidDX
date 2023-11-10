#pragma once
#include "Vertex.h"

class RectArk
{
public:
	RectArk(float x, float y, float width, float height, D3DXCOLOR color);
	~RectArk();

	float magnitudeDiagonal();

	float getX();
	float getY();
	float getWidth();
	float getHeight();

protected:
	float x, y, width, height;
	D3DXCOLOR color;

};