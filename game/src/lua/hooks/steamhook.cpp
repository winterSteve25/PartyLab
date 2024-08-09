#include "steamhook.h"

#include <isteamuser.h>

#include "raylib.h"
#include "core/Core.h"
#include "network/GameLobby.h"
#include "steam/SteamIDWrapper.h"

template <typename Func>
static void AddCppFunc(sol::state* state, const std::string& name, Func&& fn)
{
    state->set_function("cpp_" + name, fn);
}

void lua_steam_hook::AddCppTypes(sol::state* state, bool privileged)
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
            [](const SteamIDWrapper& id)
            {
                return GameLobby(id);
            }
        ),
        "getHost", [](const GameLobby& gameLobby)
        {
            return static_cast<SteamIDWrapper>(gameLobby.GetHost());
        },
        "sendChatString", &GameLobby::SendChatString,
        "getAllMembers", [](const GameLobby& gameLobby)
        {
            std::vector<SteamIDWrapper> v;
            gameLobby.GetAllMembers(&v);
            return sol::as_table(v);
        },
        "getMemberCount", &GameLobby::GetMemberCount,
        "getData", &GameLobby::GetData,
        "setData", &GameLobby::SetData
    );
}

void lua_steam_hook::AddCppFuncs(sol::state* state, bool privilege, const std::filesystem::path& modDir)
{
    // Steam API
    AddCppFunc(state, "getSteamAvatar_Large", [](const SteamIDWrapper& sid)
    {
        return SteamFriends()->GetLargeFriendAvatar(sid);
    });
    
    AddCppFunc(state, "getSteamAvatar_Medium", [](const SteamIDWrapper& sid)
    {
        return SteamFriends()->GetMediumFriendAvatar(sid);
    });
    
    AddCppFunc(state, "getSteamAvatar_Small", [](const SteamIDWrapper& sid)
    {
        return SteamFriends()->GetSmallFriendAvatar(sid);
    });
    
    AddCppFunc(state, "getCurrentUserID", []()
    {
        SteamIDWrapper x = SteamUser()->GetSteamID();
        return x;
    });

    AddCppFunc(state, "getCurrentLobby", []()
    {
        return GameLobby::CURRENT_LOBBY;
    });
    
    AddCppFunc(state, "getSteamUsername", [](const SteamIDWrapper& id)
    {
        return SteamFriends()->GetFriendPersonaName(id);
    });
    
    AddCppFunc(state, "getFriends", []()
    {
        int count = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);
        std::vector<SteamIDWrapper> lst;
        lst.reserve(count);
    
        for (int i = 0; i < count; i++)
        {
            lst.emplace_back(SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate));
        }
    
        return sol::as_table(lst);
    });

    AddCppFunc(state, "isPlayingSameGame", [](const SteamIDWrapper& id)
    {
        FriendGameInfo_t info;
        if (SteamFriends()->GetFriendGamePlayed(id, &info))
        {
            if (info.m_gameID.ToUint64() == 480) return true;
        }

        return false;
    });

    AddCppFunc(state, "getFriendLobby", [](const SteamIDWrapper& id)
    {
        FriendGameInfo_t info;
        if (SteamFriends()->GetFriendGamePlayed(id, &info) && info.m_steamIDLobby.IsValid())
        {
            return sol::optional<SteamIDWrapper>(info.m_steamIDLobby);
        }

        return sol::optional<SteamIDWrapper>(sol::nullopt);
    });
    
    if (!privilege) return;
    AddCppFunc(state, "hostLobby", []() { Core::INSTANCE->networkManager.HostLobby(); });
    AddCppFunc(state, "leaveLobby", []() { Core::INSTANCE->networkManager.LeaveCurrentLobby(); });
    AddCppFunc(state, "joinLobby", [](const SteamIDWrapper& id) { Core::INSTANCE->networkManager.JoinLobby(id); });
}

