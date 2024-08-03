#include "lua_hook.h"

#include "networkhook.h"
#include "steamhook.h"
#include "core/Core.h"
#include "lua/async/WaitFor.h"
#include "ui/Transition.h"
#include "ui/ui_helper.h"

template <typename Func>
static void AddCppFunc(sol::state* state, const std::string& name, Func&& fn)
{
    state->set_function("cpp_" + name, fn);
}

void lua_hook::AddCppTypes(sol::state* state, bool privileged)
{
    lua_steam_hook::AddCppTypes(state, privileged);
    lua_network_hook::AddCppTypes(state, privileged);
    
    state->new_usertype<Vector2>(
        "Vector2",
        sol::call_constructor,
        sol::factories(
            [](const double& x, const double& y)
            {
                return std::make_shared<Vector2>(x, y);
            },
            []()
            {
                constexpr Vector2 v{0, 0};
                return v;
            }
        ),

        "x", &Vector2::x,
        "y", &Vector2::y,
        "newX", [](const Vector2& v, const double& b)
        {
            Vector2 r = v;
            r.x = static_cast<float>(b);
            return r;
        },
        "newY", [](const Vector2& v, const double& b)
        {
            Vector2 r = v;
            r.y = static_cast<float>(b);
            return r;
        },

        "dot", [](const Vector2& self, const Vector2& other)
        {
            return self.x * other.x + self.y * other.y;
        },
        sol::meta_function::addition, [](const Vector2& a, const Vector2& b)
        {
            Vector2 r = {a.x + b.x, a.y + b.y};
            return r;
        },
        sol::meta_function::subtraction, [](const Vector2& a, const Vector2& b)
        {
            Vector2 r = {a.x - b.x, a.y - b.y};
            return r;
        },
        sol::meta_function::equal_to, [](const Vector2& a, const Vector2& b)
        {
            return a.x == b.x && a.y == b.y;
        }
    );

    state->new_usertype<Color>(
        "Color",
        sol::call_constructor,
        sol::factories([](const double& r, const double& g, const double& b, const double& a)
        {
            return std::make_shared<Color>(r, g, b, a);
        }),
        "r", &Color::r,
        "g", &Color::g,
        "b", &Color::b,
        "a", &Color::a,
        "newR", [](const Color& col, const double& r)
        {
            Color c = col;
            c.r = static_cast<unsigned char>(r);
            return c;
        },
        "newG", [](const Color& col, const double& g)
        {
            Color c = col;
            c.g = static_cast<unsigned char>(g);
            return c;
        },
        "newB", [](const Color& col, const double& b)
        {
            Color c = col;
            c.b = static_cast<unsigned char>(b);
            return c;
        },
        "newA", [](const Color& col, const double& a)
        {
            Color c = col;
            c.a = static_cast<unsigned char>(a);
            return c;
        }
    );

    state->new_usertype<Transition>(
        "Transition",
        sol::call_constructor,
        sol::factories(
            [](const unsigned int& duration, const sol::object& ease)
            {
                return std::make_shared<Transition>(duration, ease);
            }
        )
    );

    state->new_usertype<WaitFor>(
        "WaitFor",
        sol::call_constructor,
        sol::factories(
            [](const uint32_t type)
            {
                return std::make_shared<WaitFor>(static_cast<LuaWaitForEvent>(type));
            }
        )
    );
}

void lua_hook::AddCppFuncs(sol::state* state, bool privileged, const std::filesystem::path& modDir)
{
    lua_steam_hook::AddCppFuncs(state, privileged, modDir);
    lua_network_hook::AddCppFuncs(state, privileged, modDir);

    // Logging
    AddCppFunc(state, "info", [](const char* text) { TraceLog(LOG_INFO, text); });
    AddCppFunc(state, "warning", [](const char* text) { TraceLog(LOG_WARNING, text); });
    AddCppFunc(state, "error", [](const char* text) { TraceLog(LOG_ERROR, text); });

    // UI Helper
    AddCppFunc(state, "getScreenWidth", GetScreenWidth);
    AddCppFunc(state, "getScreenHeight", GetScreenHeight);

    AddCppFunc(state, "measureText", ui_helper::MeasureText);
    AddCppFunc(state, "getCenter", ui_helper::GetCenter);
    AddCppFunc(state, "within", ui_helper::Within);

    // Core
    AddCppFunc(state, "transitionTo", [](int scene)
    {
        Core::INSTANCE->transitionManager.TransitionTo(
            scene, sol::optional<sol::protected_function>(sol::nullopt));
    });
    AddCppFunc(state, "transitionToCB", [](int scene, const sol::protected_function& callback)
    {
        Core::INSTANCE->transitionManager.TransitionTo(scene, callback);
    });
    AddCppFunc(state, "exit", []() { Core::INSTANCE->shouldExit = true; });

    // Raylib
    AddCppFunc(state, "unloadTexture", [](const int& handle)
    {
        return Core::INSTANCE->resourceManager.UnloadTex(handle);
    });
    AddCppFunc(state, "loadTexture", [modDir](const std::string& file)
    {
        std::filesystem::path path = modDir / file;
        return Core::INSTANCE->resourceManager.LoadTex(path.generic_string());
    });
    AddCppFunc(state, "drawTexture", [](const int& handle, const Vector2& pos, const Vector2& size)
    {
        Texture2D tex = Core::INSTANCE->resourceManager.GetTex(handle);

        DrawTexturePro(
            tex,
            {0, 0, static_cast<float>(tex.width), static_cast<float>(tex.height)},
            {pos.x, pos.y, size.x, size.y},
            {0, 0},
            0,
            WHITE
        );
    });
    AddCppFunc(state, "getTextureSize", [](const int& handle)
    {
        Texture2D tex = Core::INSTANCE->resourceManager.GetTex(handle);
        return Vector2(static_cast<float>(tex.width), static_cast<float>(tex.height));
    });
}
