#include "Renderer.h"

#include "raylib.h"

void Renderer::render(float deltaTime)
{
    ClearBackground(WHITE);
    DrawText("hello", 200, 200, 20, BLACK);
}
