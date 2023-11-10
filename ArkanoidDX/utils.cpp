#include "utils.h"

std::pair<float, float> getCoordinateRelative(float x, float y, float screenWidth, float screenHeight) {
	float xTmp = x * 2 / screenWidth;
	xTmp -= 1;
	
	float yTmp = y * -2 / screenHeight;
	yTmp += 1;

	return { xTmp, yTmp };
}