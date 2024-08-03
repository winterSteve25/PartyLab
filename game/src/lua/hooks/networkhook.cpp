#include "networkhook.h"

#include "raylib.h"
#include "core/Core.h"
#include "network/GameLobby.h"
#include "network/NetworkTarget.h"
#include "steam/SteamIDWrapper.h"

template <typename Func>
static void AddCppFunc(sol::state* state, const std::string& name, Func&& fn)
{
    state->set_function("cpp_" + name, fn);
}

void lua_network_hook::AddCppTypes(sol::state* state, bool privileged)
{
    state->new_usertype<SteamIDWrapper>(
        "SteamID",
        sol::no_constructor,
        sol::meta_function::equal_to, [](const SteamIDWrapper& a, const SteamIDWrapper& b)
        {
            const CSteamID ac = a;
            const CSteamID ab = b;
            return ac.ConvertToUint64() == ab.ConvertToUint64();
        },
        "equals", [](const SteamIDWrapper& id, const SteamIDWrapper& other)
        {
            TraceLog(LOG_INFO, std::to_string(static_cast<CSteamID>(id).ConvertToUint64()).c_str());
            const CSteamID ac = id;
            const CSteamID ab = other;
            return ac.ConvertToUint64() == ab.ConvertToUint64();
        },
        "id", &SteamIDWrapper::unAccountId
    );

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
        },
        "getData", [](const GameLobby& gameLobby, const std::string& key)
        {
            return gameLobby.GetData(key);
        },
        "setData", [](const GameLobby& gameLobby, const std::string& key, const std::string& val)
        {
            gameLobby.SetData(key, val);
        }
    );
}

void lua_network_hook::AddCppFuncs(sol::state* state, bool privilege, const std::filesystem::path& modDir)
{
    AddCppFunc(state, "sendPacket", [](const int& target, const int& type, const sol::object& data, const int& flags)
    {
        Core::INSTANCE->networkManager.SendMessage(static_cast<NetworkTarget>(target), std::optional<SteamIDWrapper>(std::nullopt), type, data, flags);
    });
    
    AddCppFunc(state, "sendPacketTo", [](const SteamIDWrapper& target, const int& type, const sol::object& data, const int& flags)
    {
        Core::INSTANCE->networkManager.SendMessage(PLAYER, target, type, data, flags);
    });
}

