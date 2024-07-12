#include "raylib.h"
#include "rendering/Renderer.h"
#define LAY_IMPLEMENTATION
#include "ui/layout.h"

int main ()
{
	Renderer renderer;
	SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 800, "Party Lab");
	SetTargetFPS(60);
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		renderer.render(GetFrameTime());
		EndDrawing();
	}

	CloseWindow();
}
