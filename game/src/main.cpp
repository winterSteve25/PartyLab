#include "raylib.h"
#include "core/Core.h"

int main ()
{
	Core updater;
	SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 800, "Party Lab");
	SetTargetFPS(60);

	Camera2D camera;
	camera.zoom = 1;
	camera.rotation = 0;
	camera.offset = { 0, 0 };

	while (!WindowShouldClose())
	{
		BeginDrawing();
		updater.Update();
		BeginMode2D(camera);
		updater.Render(camera);
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
}
