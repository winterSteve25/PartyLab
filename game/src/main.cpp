#include "raylib.h"
#include "core/Core.h"

#define LAY_IMPLEMENTATION
#include "layout.h"

int main ()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetExitKey(KEY_NULL);
	InitWindow(1280, 800, "Party Lab");
	SetTargetFPS(60);

	Camera2D camera;
	camera.zoom = 1;
	camera.rotation = 0;
	camera.offset = { 0, 0 };
	Core core(0);

	while (!WindowShouldClose() || core.shouldExit)
	{
		BeginDrawing();
		core.Update();
		core.Render(camera);
		EndDrawing();
	}

	CloseWindow();
}
