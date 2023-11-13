#pragma once
#include <utility>
#include "globalDeclarations.h"
#include "Components.h"
#include "gameOptions.h"
#include <chrono>
#include <algorithm>

std::pair<float, float> getCoordinateRelative(float x, float y, float screenWidth, float screenHeight);

void InitComponents(VERTEX Vertices[]);
void InsertBall(VERTEX*& Vertices);
void InsertPlatform(VERTEX*& Vertices);
void InsertBlocks(VERTEX*& Vertices);
void UpdateBallPosition(VERTEX* Vertices);
void UpdatePlatformPosition(VERTEX* Vertices);
void CheckCollisions(VERTEX* Vertices, HWND hWnd);
void CheckScreenCollision(HWND hWnd);
void CheckBlocksCollision(VERTEX* Vertices);
void CheckPlatformCollision();
void ResetLevel(VERTEX* Vertices);