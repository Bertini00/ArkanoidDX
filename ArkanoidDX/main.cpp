// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "Timer.h"
#include "utils.h"
#include "gameOptions.h"
#include <vector>
#include "Components.h"
#include <stdlib.h>
#include <chrono>
#include <algorithm>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SCREEN_SAFE_TIME 0.3f


#define STARTING_BALL_X SCREEN_WIDTH / 2 - 20
#define STARTING_BALL_Y SCREEN_HEIGHT - 20 - PLATFORM_HEIGHT
#define STARTING_BALL_VELOCITY_X 150
#define STARTING_BALL_VELOCITY_Y -150

#define STARTING_PLATFORM_X (SCREEN_WIDTH - PLATFORM_WIDTH) / 2
#define STARTING_PLATFORM_Y SCREEN_HEIGHT - PLATFORM_HEIGHT
#define PLATFORM_HEIGHT 30
#define PLATFORM_WIDTH 200
#define PLATFORM_SPEED 400
#define PLATFORM_SAFE_TIME 0.5f

#define BLOCK_NUMBER 8
#define BLOCK_PADDING 2
#define BLOCK_WIDTH_NO_PADDING 100
#define BLOCK_HEIGHT_NO_PADDING 40
#define BLOCK_WIDTH BLOCK_WIDTH_NO_PADDING - 2*BLOCK_PADDING 
#define BLOCK_HEIGHT BLOCK_HEIGHT_NO_PADDING - 2*BLOCK_PADDING

#define FINAL_VERTEX_SIZE 6 * (2 + BLOCK_NUMBER)

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
ID3D11InputLayout* pLayout;            // the pointer to the input layout
ID3D11VertexShader* pVS;               // the pointer to the vertex shader
ID3D11PixelShader* pPS;                // the pointer to the pixel shader
ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer


Ball* ball;
Platform* platform;

std::vector<RectArk*> blockVector(BLOCK_NUMBER);
std::vector<bool> hitVector(BLOCK_NUMBER);
VERTEX OurVertices[FINAL_VERTEX_SIZE];

float platformSafeTime = 0;
float screenSafeTime = 0;

int hitBlocks = 0;
bool gameOver = false;


// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders

void InitComponents(VERTEX Vertices[]);
void InsertBall(VERTEX *&Vertices);
void InsertPlatform(VERTEX *&Vertices);
void InsertBlocks(VERTEX *&Vertices);
void UpdateBallPosition(VERTEX* Vertices);
void UpdatePlatformPosition(VERTEX* Vertices);
void CheckCollisions(VERTEX* Vertices, HWND hWnd);
void CheckScreenCollision(HWND hWnd);
void CheckBlocksCollision(VERTEX* Vertices);
void CheckPlatformCollision();
void ResetLevel(VERTEX* Vertices);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
        L"WindowClass",
        L"Our First Direct3D Program",
        WS_OVERLAPPEDWINDOW,
        300,
        300,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

    // enter the main loop:

    MSG msg;

    Timer* t = new Timer();
    
    while (TRUE)
    {
        t->tick();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }
        if (GetKeyState('A') & 0x8000 && platform->getX() > 0)
        {
            platform->changeDirection(-1);
        }
        if (GetKeyState('D') & 0x8000 && platform->getX() + platform->getWidth() < SCREEN_WIDTH)
        {
            platform->changeDirection(1);
        }
        if (GetKeyState('P') & 0x8000)
        {
            break;
        }
        /*
        */
        CheckCollisions(OurVertices, hWnd);

        ball->integrate(t->getDeltaTime());
        platform->integrate(t->getDeltaTime());
        UpdateBallPosition(OurVertices);
        UpdatePlatformPosition(OurVertices);
        platformSafeTime += t->getDeltaTime();
        screenSafeTime += t->getDeltaTime();


        if (hitBlocks == BLOCK_NUMBER)
        {
            ResetLevel(OurVertices);
            //break;
        }
            
        RenderFrame();

    }
        

    // clean up DirectX and COM
    CleanD3D();

    delete t;

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);


    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(void)
{
    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * FINAL_VERTEX_SIZE;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    devcon->Draw(FINAL_VERTEX_SIZE, 0);

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
    
    delete ball;
    delete platform;
    
    for (int i = 0; i < blockVector.size(); i++)
    {
        delete blockVector[i];
    }

    blockVector.clear();
    hitVector.clear();
    
}


// this is the function that creates the shape to render
void InitGraphics()
{
    // create a triangle using the VERTEX struct
    /*
    VERTEX OurVertices[] =
    {
        {-1.0f, 1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {-0.94f,  1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {-1.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {-1.0f, -0.93f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {-0.95f, -1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {-0.95f, -0.93f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)}
    };
    */
    
    
    // BLOCK_NUMBER + BALL + PAD
    //VERTEX OurVertices[(BLOCK_NUMBER + 2)*6];
    //VERTEX OurVertices[6];
    InitComponents(OurVertices);

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * FINAL_VERTEX_SIZE;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}


// this function loads and prepares the shaders
void InitPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* VS, * PS;
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}



void InitComponents(VERTEX Vertices[]) {
    InsertBall(Vertices);
    InsertPlatform(Vertices);
    InsertBlocks(Vertices);
}

void InsertBall(VERTEX *&Vertices) {
    ball = new Ball(STARTING_BALL_X, STARTING_BALL_Y, 20, 20, {0.0f, 1.0f, 0.0f, 1.0f}, STARTING_BALL_VELOCITY_X, STARTING_BALL_VELOCITY_Y);
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
        gameOver = true;
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
            p = std::clamp(p, 0.0f, 0.75f);
            newXVelocity *= p * totalVelocity;
            newYVelocity *= (1 - p) * totalVelocity;
        }
        else {
            // On the left of the platform
            float p = 0;

            float relativeX = (ball->getX() + ball->getWidth() / 2) - (platform->getX() + platform->getWidth() / 2);
            p = abs(relativeX / (PLATFORM_WIDTH / 2));
            p = std::clamp(p, 0.0f, 0.75f);
            newXVelocity *= p * totalVelocity;
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