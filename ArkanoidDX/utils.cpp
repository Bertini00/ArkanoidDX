#include "utils.h"

/*
* The top left corner has coordinate {0, 0}
* The bottom right corner has coordinate {screenWidth, screenHeight}
*/
std::pair<float, float> getCoordinateRelative(float x, float y, float screenWidth, float screenHeight) {
	float xTmp = x * 2 / screenWidth;
	xTmp -= 1;
	
	float yTmp = y * -2 / screenHeight;
	yTmp += 1;

	return { xTmp, yTmp };
}


void InitComponents(VERTEX Vertices[]) {
    InsertBall(Vertices);
    InsertPlatform(Vertices);
    InsertBlocks(Vertices);
}

void InsertBall(VERTEX *&Vertices) {
    ball = new Ball(STARTING_BALL_X, STARTING_BALL_Y, BALL_WIDTH, BALL_HEIGHT, {0.0f, 1.0f, 0.0f, 1.0f}, STARTING_BALL_VELOCITY_X, STARTING_BALL_VELOCITY_Y);
    std::pair<float, float> tmpS = getCoordinateRelative(ball->getX(), ball->getY(), SCREEN_WIDTH, SCREEN_HEIGHT);
    std::pair<float, float> tmpWH = getCoordinateRelative(ball->getX() + ball->getWidth(), ball->getY() + ball->getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT);
    *Vertices = { tmpS.first, tmpS.second, 0.0f, ball->getColor()};
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpWH.second, 0.0f, ball->getColor() };
    Vertices++;
    
    
}
void UpdateBallPosition(VERTEX* Vertices) {
    std::pair<float, float> tmpS = getCoordinateRelative(ball->getX(), ball->getY(), SCREEN_WIDTH, SCREEN_HEIGHT);
    std::pair<float, float> tmpWH = getCoordinateRelative(ball->getX() + ball->getWidth(), ball->getY() + ball->getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT);
    *Vertices = { tmpS.first, tmpS.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, ball->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpWH.second, 0.0f, ball->getColor() };
    Vertices++;
}

void InsertPlatform(VERTEX *&Vertices) {
    platform = new Platform(STARTING_PLATFORM_X , STARTING_PLATFORM_Y, 200, PLATFORM_HEIGHT, { 1.0f, 0.0f, 0.0f, 1.0f }, PLATFORM_SPEED);
    std::pair<float, float> tmpS = getCoordinateRelative(platform->getX(), platform->getY(), SCREEN_WIDTH, SCREEN_HEIGHT);
    std::pair<float, float> tmpWH = getCoordinateRelative(platform->getX() + platform->getWidth(), platform->getY() + platform->getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT);
    *Vertices = { tmpS.first, tmpS.second, 0.0f, platform->getColor()};
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpWH.second, 0.0f, platform->getColor() };
    Vertices++;
}

void UpdatePlatformPosition(VERTEX* Vertices) {
    // Platform being always the second object, advance the pointer by 6
    Vertices += 6;
    //TMPS = TempStart -> The top left point of the Rectangle
    //TMPWH = TempWidthHeight -> The bottom right point of the Rectangle

    std::pair<float, float> tmpS = getCoordinateRelative(platform->getX(), platform->getY(), SCREEN_WIDTH, SCREEN_HEIGHT);
    std::pair<float, float> tmpWH = getCoordinateRelative(platform->getX() + platform->getWidth(), platform->getY() + platform->getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT);
    *Vertices = { tmpS.first, tmpS.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpS.first, tmpWH.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpS.second, 0.0f, platform->getColor() };
    Vertices++;
    *Vertices = { tmpWH.first, tmpWH.second, 0.0f, platform->getColor() };
    Vertices++;
}

void InsertBlocks(VERTEX *&Vertices) {
    RectArk* block;
    float currentX = 0;
    float currentY = 0;
    //D3DXCOLOR color = { 0.8f, 0.4f, 1.0f, 1.0f };
    std::pair<float, float> tmpS;
    std::pair<float, float> tmpWH;
    D3DXCOLOR color;
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    blockVector.clear();
    for (int i = 0; i < BLOCK_NUMBER; i++)
    {
        color = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f};
        block = new RectArk(currentX + BLOCK_PADDING, currentY + BLOCK_PADDING, BLOCK_WIDTH, BLOCK_HEIGHT, color);
        blockVector.push_back(block);
        hitVector.push_back(false);

        tmpS = getCoordinateRelative(block->getX(), block->getY(), SCREEN_WIDTH, SCREEN_HEIGHT);
        tmpWH = getCoordinateRelative(block->getX() + block->getWidth(), block->getY() + block->getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT);

        // Creating visual rectangle
        *Vertices = { tmpS.first, tmpS.second, 0.0f, block->getColor() };
        Vertices++;
        *Vertices = { tmpWH.first, tmpS.second, 0.0f, block->getColor() };
        Vertices++;
        *Vertices = { tmpS.first, tmpWH.second, 0.0f, block->getColor() };
        Vertices++;
        *Vertices = { tmpS.first, tmpWH.second, 0.0f, block->getColor() };
        Vertices++;
        *Vertices = { tmpWH.first, tmpS.second, 0.0f, block->getColor() };
        Vertices++;
        *Vertices = { tmpWH.first, tmpWH.second, 0.0f, block->getColor() };
        Vertices++;

        currentX += BLOCK_WIDTH_NO_PADDING;
        if (currentX > SCREEN_WIDTH - BLOCK_WIDTH_NO_PADDING)
        {
            currentX = 0;
            currentY += BLOCK_HEIGHT_NO_PADDING;
        }

    }
}

void CheckCollisions(VERTEX* Vertices, HWND hWnd) {
    CheckScreenCollision(hWnd);
    CheckBlocksCollision(Vertices);
    CheckPlatformCollision();
}

void CheckScreenCollision(HWND hWnd) {
    if ((ball->getX() < 0 || ball->getX() > (SCREEN_WIDTH - ball->getWidth())) && screenSafeTime > SCREEN_SAFE_TIME)
    {
        ball->horizontalCollision();
        screenSafeTime = 0;
    }
    else if (ball->getY() < 0 && screenSafeTime > SCREEN_SAFE_TIME)
    {
        ball->verticalCollision();
        screenSafeTime = 0;
    }
    if (ball->getY() > (SCREEN_HEIGHT - ball->getHeight())) {
        PostMessage(hWnd, WM_QUIT, 0, 0);
    }
}

void CheckBlocksCollision(VERTEX* Vertices) {
    Vertices += 2 * 6;
    for (int i = 0; i < blockVector.size(); i++)
    {
        RectArk* currentBlock = blockVector[i];
        if (!hitVector[i])
        {
            if (ball->getX() > currentBlock->getX() - ball->getWidth() &&
                ball->getX() < currentBlock->getX() + currentBlock->getWidth() &&
                ball->getY() > currentBlock->getY() - ball->getHeight() &&
                ball->getY() < currentBlock->getY() + currentBlock->getHeight())
            {
                *Vertices = { 0.0f, 0.0f, 0.0f, currentBlock->getColor() };
                Vertices++;
                *Vertices = { 0.0f, 0.0f, 0.0f, currentBlock->getColor() };
                Vertices++;
                *Vertices = { 0.0f, 0.0f, 0.0f, currentBlock->getColor() };
                Vertices++;
                *Vertices = { 0.0f, 0.0f, 0.0f, currentBlock->getColor() };
                Vertices++;
                *Vertices = { 0.0f, 0.0f, 0.0f, currentBlock->getColor() };
                Vertices++;
                *Vertices = { 0.0f, 0.0f, 0.0f, currentBlock->getColor() };
                Vertices++;
                hitVector[i] = true;

                hitBlocks++;
                

                if (min(ball->getX() - (currentBlock->getX() - ball->getWidth()), ball->getX() - (currentBlock->getX() + currentBlock->getWidth())) <
                    min(ball->getY() - (currentBlock->getY() - ball->getHeight()), ball->getY() - (currentBlock->getY() + currentBlock->getHeight()))
                    )
                {
                    ball->verticalCollision();
                }
                else
                    ball->horizontalCollision();

                
            }
            else
                Vertices += 6;
        }
        else
            Vertices += 6;

    }
}

void CheckPlatformCollision() {
    if (ball->getY() + ball->getHeight() > platform->getY() && 
        ball->getX() > platform->getX() - ball->getWidth() && 
        ball->getX() < platform->getX() + platform->getWidth() && 
        platformSafeTime > PLATFORM_SAFE_TIME)
    {
        int totalVelocity = abs(STARTING_BALL_VELOCITY_X) + abs(STARTING_BALL_VELOCITY_Y);

        float newXVelocity = 1;
        float newYVelocity = 1;

        if ((ball->getX() + ball->getWidth()/2) - (platform->getX() + platform->getWidth()/2) > 0 )
        {
            // On the right of the platform
            float p = 0;

            float relativeX = (ball->getX() + ball->getWidth() / 2) - (platform->getX() + platform->getWidth());
            p = abs(relativeX / (PLATFORM_WIDTH / 2));
            p = std::clamp(p, 0.25f, 1.0f);
            newXVelocity *= (1 - p) * totalVelocity;
            newYVelocity *= (p) * totalVelocity;
        }
        else {
            // On the left of the platform
            float p = 0;

            float relativeX = (ball->getX() + ball->getWidth() / 2) - (platform->getX() + platform->getWidth() / 2);
            p = abs(relativeX / (PLATFORM_WIDTH / 2));
            p = std::clamp(p, 0.0f, 0.75f);
            newXVelocity *= (p) * totalVelocity;
            newYVelocity *= (1 - p) * totalVelocity;
            newXVelocity *= -1;
        }

        ball->setVelocityX(newXVelocity);
        ball->setVelocityY(newYVelocity);

        platformSafeTime = 0;
        ball->verticalCollision();
    }
}

void ResetLevel(VERTEX* Vertices) {
    for (int i = 0; i < hitVector.size(); i++)
    {
        hitVector[i] = false;
    }
    hitBlocks = 0;

    
    ball->setX(STARTING_BALL_X);
    ball->setY(STARTING_BALL_Y);
    ball->setVelocityX(STARTING_BALL_VELOCITY_X);
    ball->setVelocityY(STARTING_BALL_VELOCITY_Y);
    

    Vertices += 2 * 6;
    RectArk* block;
    float currentX = 0;
    float currentY = 0;
    //D3DXCOLOR color = { 0.8f, 0.4f, 1.0f, 1.0f };
    std::pair<float, float> tmpS;
    std::pair<float, float> tmpWH;
    D3DXCOLOR color;
    
    for (int i = 0; i < BLOCK_NUMBER; i++)
    {
        color = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f };
        block = blockVector[i];

        tmpS = getCoordinateRelative(block->getX(), block->getY(), SCREEN_WIDTH, SCREEN_HEIGHT);
        tmpWH = getCoordinateRelative(block->getX() + block->getWidth(), block->getY() + block->getHeight(), SCREEN_WIDTH, SCREEN_HEIGHT);

        // Creating visual rectangle
        *Vertices = { tmpS.first, tmpS.second, 0.0f, color};
        Vertices++;
        *Vertices = { tmpWH.first, tmpS.second, 0.0f, color };
        Vertices++;
        *Vertices = { tmpS.first, tmpWH.second, 0.0f, color };
        Vertices++;
        *Vertices = { tmpS.first, tmpWH.second, 0.0f, color };
        Vertices++;
        *Vertices = { tmpWH.first, tmpS.second, 0.0f, color };
        Vertices++;
        *Vertices = { tmpWH.first, tmpWH.second, 0.0f, color };
        Vertices++;

        currentX += BLOCK_WIDTH_NO_PADDING;
        if (currentX > SCREEN_WIDTH - BLOCK_WIDTH_NO_PADDING)
        {
            currentX = 0;
            currentY += BLOCK_HEIGHT_NO_PADDING;
        }

    }
    
}