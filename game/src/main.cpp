#include "raylib.h"
#include "core/Core.h"
#include "steam_api.h"

#define LAY_IMPLEMENTATION
#include "layout.h"
#include "steam/SteamEvents.h"

// int main()
// {
//     sol::state state;
//     state.open_libraries(sol::lib::base);
//     sol::object obj = state.script("return { hallo = 12901.128, \"josh\", { 1, 2, 3, false } }");
//
//     MemoryWriter writer;
//
//     writer.WriteGeneric<float>(12981.21);
//     writer.WriteGeneric<double>(12981.219410);
//     writer.WriteString("hallo there");
//     writer.WriteGeneric<uint32_t>(219410);
//     writer.WriteString("hallo there 2");
//     writer.WriteString("hallo there 3");
//     writer.WriteBool(true);
//     writer.WriteObject(obj);
//     writer.WriteBool(false);
//     writer.WriteString("hallo there 4");
//     writer.WriteBool(true);
//
//     MemoryReader reader(writer.GetPtr(), writer.GetSize());
//
//     std::cout << std::format("{}", reader.ReadGeneric<float>()) << std::endl;
//     std::cout << std::format("{}", reader.ReadGeneric<double>()) << std::endl;
//     std::cout << std::format("{}", reader.ReadString()) << std::endl;
//     std::cout << std::format("{}", reader.ReadGeneric<uint32_t>()) << std::endl;
//     std::cout << std::format("{}", reader.ReadString()) << std::endl;
//     std::cout << std::format("{}", reader.ReadString()) << std::endl;
//     std::cout << std::format("{}", reader.ReadBool()) << std::endl;
//     state["Test"] = reader.ReadObject(state.lua_state());
//      sol::object s = state.script(R"V0G0N(
//          -- Function to format a table as a string
//          function format_table(t)
//              local result = "{\n"
//              for k, v in pairs(t) do
//                  -- Format key-value pairs
//                 if type(v) == "table" then
//                     result = result .. " " .. tostring(k) .. " = " .. format_table(v) .. ",\n"
//                 else
//                     result = result .. "  " .. tostring(k) .. " = " .. tostring(v) .. ",\n"
//                 end
//              end
//              result = result .. "}"
//              return result
//          end
//
//          -- Get the formatted string of the table
//          local formattedTable = format_table(Test)
//
//          -- Return the formatted table string
//          return formattedTable
//      )V0G0N");
//     std::cout << std::format("{}", s.as<std::string>()) << std::endl;
//     std::cout << std::format("{}", reader.ReadBool()) << std::endl;
//     std::cout << std::format("{}", reader.ReadString()) << std::endl;
//     std::cout << std::format("{}", reader.ReadBool()) << std::endl;
// }

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
