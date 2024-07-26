#include "lua_hook.h"

#include <memory>

#include "raylib.h"
#include "core/Core.h"
#include "ui/Size.h"
#include "ui/Transition.h"
#include "ui/ui_helper.h"

template <typename Func>
void AddCPPFunc(sol::state* state, const std::string& name, Func&& fn)
{
    state->set_function("cpp_" + name, fn);
}

void lua_hook::AddCppTypes(sol::state* state, bool privileged)
{
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

        "dot",
        [](const Vector2& self, const Vector2& other)
        {
            return self.x * other.x + self.y * other.y;
        },

        sol::meta_function::addition,
        [](const Vector2& a, const Vector2& b)
        {
            Vector2 r = {a.x + b.x, a.y + b.y};
            return r;
        },

        sol::meta_function::subtraction,
        [](const Vector2& a, const Vector2& b)
        {
            Vector2 r = {a.x - b.x, a.y - b.y};
            return r;
        },

        sol::meta_function::equal_to,
        [](const Vector2& a, const Vector2& b)
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
        "r", [](const Color& col, const double& r)
        {
            Color c = col;
            c.r = static_cast<unsigned char>(r);
            return c;
        },
        "g", [](const Color& col, const double& g)
        {
            Color c = col;
            c.g = static_cast<unsigned char>(g);
            return c;
        },
        "b", [](const Color& col, const double& b)
        {
            Color c = col;
            c.b = static_cast<unsigned char>(b);
            return c;
        },
        "a", [](const Color& col, const double& a)
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
}

void lua_hook::AddCppFuncs(sol::state* state, bool privileged)
{
    AddCPPFunc(state, "info", [](const char* text) { TraceLog(LOG_INFO, text); });
    AddCPPFunc(state, "warning", [](const char* text) { TraceLog(LOG_WARNING, text); });
    AddCPPFunc(state, "error", [](const char* text) { TraceLog(LOG_ERROR, text); });

    AddCPPFunc(state, "getScreenWidth", GetScreenWidth);
    AddCPPFunc(state, "getScreenHeight", GetScreenHeight);

    AddCPPFunc(state, "measureText", ui_helper::MeasureText);
    AddCPPFunc(state, "getCenter", ui_helper::GetCenter);
    AddCPPFunc(state, "within", ui_helper::Within);

    AddCPPFunc(state, "transitionTo", [](int scene) { Core::INSTANCE->transitionManager.TransitionTo(scene); });
    AddCPPFunc(state, "exit", []() { Core::INSTANCE->shouldExit = true; });
}
