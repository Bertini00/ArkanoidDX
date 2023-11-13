#include "globalDeclarations.h"

Ball* ball;
Platform* platform;

std::vector<RectArk*> blockVector(BLOCK_NUMBER);
std::vector<bool> hitVector(BLOCK_NUMBER);
VERTEX OurVertices[FINAL_VERTEX_SIZE];

float platformSafeTime = 0;
float screenSafeTime = 0;

int hitBlocks = 0;
