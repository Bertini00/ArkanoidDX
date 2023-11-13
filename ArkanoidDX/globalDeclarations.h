#pragma once
#include "Components.h"
#include <vector>
#include "gameOptions.h"



#ifndef GLOBALS_DEC
#define GLOBALS_DEC 1


extern Ball* ball;
extern Platform* platform;

extern std::vector<RectArk*> blockVector;
extern std::vector<bool> hitVector;
extern VERTEX OurVertices[FINAL_VERTEX_SIZE];

extern float platformSafeTime;
extern float screenSafeTime;

extern int hitBlocks;

#endif // GLOBALS_DEC