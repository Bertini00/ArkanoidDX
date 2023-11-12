#include "RectArk.h"
#include <cmath>

RectArk::RectArk(float x, float y, float width, float height, D3DXCOLOR color) {
	RectArk::x = x;
	RectArk::y = y;
	RectArk::width = width;
	RectArk::height = height;
	RectArk::color = color;
}

RectArk::~RectArk() {

}

float RectArk::magnitudeDiagonal()
{
	return sqrt(width * width + height * height);
}

float RectArk::getHeight() {
	return height;
}
float RectArk::getWidth() {
	return width;
}
float RectArk::getX() {
	return x;
}
float RectArk::getY() {
	return y;
}

D3DXCOLOR RectArk::getColor() {
	return color;
}

