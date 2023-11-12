#pragma once
#include "Vertex.h"

class RectArk
{
public:
	RectArk(float x, float y, float width, float height, D3DXCOLOR color);
	virtual ~RectArk();

	float magnitudeDiagonal();

	float getX();
	float getY();
	float getWidth();
	float getHeight();

	D3DXCOLOR getColor();

protected:
	float x, y, width, height;
	D3DXCOLOR color;

};