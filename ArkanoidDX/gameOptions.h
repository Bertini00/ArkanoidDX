#pragma once

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SCREEN_SAFE_TIME 0.1f

#define STARTING_BALL_X SCREEN_WIDTH / 2 - 20
#define STARTING_BALL_Y SCREEN_HEIGHT - 20 - PLATFORM_HEIGHT
#define STARTING_BALL_VELOCITY_X 150
#define STARTING_BALL_VELOCITY_Y -150
#define BALL_WIDTH 10
#define BALL_HEIGHT 10

#define STARTING_PLATFORM_X (SCREEN_WIDTH - PLATFORM_WIDTH) / 2
#define STARTING_PLATFORM_Y SCREEN_HEIGHT - PLATFORM_HEIGHT
#define PLATFORM_HEIGHT 30
#define PLATFORM_WIDTH 200
#define PLATFORM_SPEED 400
#define PLATFORM_SAFE_TIME 0.5f

#define BLOCK_NUMBER 24
#define BLOCK_PADDING 2
#define BLOCK_WIDTH_NO_PADDING 100
#define BLOCK_HEIGHT_NO_PADDING 40
#define BLOCK_WIDTH BLOCK_WIDTH_NO_PADDING - 2*BLOCK_PADDING 
#define BLOCK_HEIGHT BLOCK_HEIGHT_NO_PADDING - 2*BLOCK_PADDING

#define FINAL_VERTEX_SIZE 6 * (2 + BLOCK_NUMBER)