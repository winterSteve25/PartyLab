#include "lua_hook.h"

#include <isteamclient.h>
#include <isteamuser.h>
#include <isteamutils.h>
#include <memory>
#include <memory>

#include "raylib.h"
#include "async/WaitFor.h"
#include "core/Core.h"
#include "steam/SteamIDWrapper.h"
#include "ui/Length.h"
#include "ui/Transition.h"
#include "ui/ui_helper.h"

template <typename Func>
void AddCppFunc(sol::state* state, const std::string& name, Func&& fn)
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

    state->new_usertype<SteamIDWrapper>("SteamID", sol::no_constructor);
    state->new_usertype<GameLobby>(
        "GameLobby",
        sol::call_constructor,
        sol::factories(
            [](const SteamIDWrapper id)
            {
                return std::make_shared<GameLobby>(id);
            }
        ),
        "getHost", [](const GameLobby& gameLobby)
        {
            return static_cast<SteamIDWrapper>(gameLobby.GetHost());
        },
        "sendChatString", [](const GameLobby& gameLobby, const std::string& str)
        {
            gameLobby.SendChatString(str);
        },
        "getAllMembers", [](const GameLobby& gameLobby)
        {
            std::vector<SteamIDWrapper> v;
            gameLobby.GetAllMembers(&v);
            return sol::as_table(v);
        }
    );
}

void lua_hook::AddCppFuncs(sol::state* state, bool privileged)
{
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
    AddCppFunc(state, "transitionToCB", [](int scene, sol::protected_function callback)
    {
        Core::INSTANCE->transitionManager.TransitionTo(scene, callback);
    });
    AddCppFunc(state, "exit", []() { Core::INSTANCE->shouldExit = true; });

    // Steam API
    AddCppFunc(state, "getSteamAvatar_Large", [](SteamIDWrapper sid)
    {
        return SteamFriends()->GetLargeFriendAvatar(sid);
    });
    AddCppFunc(state, "getSteamAvatar_Medium", [](SteamIDWrapper sid)
    {
        return SteamFriends()->GetMediumFriendAvatar(sid);
    });
    AddCppFunc(state, "getSteamAvatar_Small", [](SteamIDWrapper sid)
    {
        return SteamFriends()->GetSmallFriendAvatar(sid);
    });
    AddCppFunc(state, "getCurrentUserID", []()
    {
        SteamIDWrapper x = SteamUser()->GetSteamID();
        return x;
    });

    AddCppFunc(state, "getCurrentLobby", []() { return GameLobby::CURRENT_LOBBY; });

    if (!privileged) return;
    AddCppFunc(state, "hostLobby", []() { Core::INSTANCE->networkManager.HostLobby(); });
    AddCppFunc(state, "leaveLobby", []() { Core::INSTANCE->networkManager.LeaveCurrentLobby(); });
    AddCppFunc(state, "joinedLobby", []() { Core::INSTANCE->networkManager.LeaveCurrentLobby(); });
}
