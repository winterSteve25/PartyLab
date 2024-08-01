#include "raylib.h"
#include "core/Core.h"
#include "steam_api.h"

#define LAY_IMPLEMENTATION
#include "layout.h"
#include "lua/LuaConstants.h"
#include "steam/SteamEvents.h"

int main()
{
    if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
    {
        return EXIT_FAILURE;
    }
    
    SteamErrMsg errMsg = {0};
    if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
    {
        TraceLog(LOG_ERROR, "SteamAPI_Init() failed: ");
        TraceLog(LOG_ERROR, errMsg);
        return EXIT_FAILURE;
    }
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 800, "Party Lab");
    SetExitKey(KEY_NULL);

    Camera2D camera;
    camera.zoom = 1;
    camera.rotation = 0;
    camera.offset = {0, 0};
    SteamEvents steamEvents;
    Core core(0);

    while (!WindowShouldClose() && !core.shouldExit)
    {
        BeginDrawing();
        SteamAPI_RunCallbacks();
        core.Update();
        core.Render(camera);
        DrawFPS(20, 20);
        EndDrawing();
    }

    SteamAPI_Shutdown();
    CloseWindow();
    return EXIT_SUCCESS;
}
