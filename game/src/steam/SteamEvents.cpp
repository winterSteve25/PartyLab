// ReSharper disable CppMemberFunctionMayBeStatic
#include "SteamEvents.h"

#include "SteamIDWrapper.h"
#include "core/Core.h"
#include "lua/LuaConstants.h"

void SteamEvents::OnAvatarLoaded(AvatarImageLoaded_t* pParam)
{
    Core::INSTANCE->luaAsyncManager.NotifyEvent(AvatarLoaded, SteamIDWrapper(pParam->m_steamID));
}

void SteamEvents::OnLobbyChatMessageReceived(LobbyChatMsg_t* pParam)
{
    CSteamID sender;
    auto data = malloc(4096);
    EChatEntryType chatEntryType;

    int readData = SteamMatchmaking()->GetLobbyChatEntry(
        pParam->m_ulSteamIDLobby,
        pParam->m_iChatID,
        &sender,
        data,
        4096,
        &chatEntryType
    );

    if (readData > 0)
    {
        const std::function sd = [&sender](sol::state_view*) { return static_cast<SteamIDWrapper>(sender); };
        const std::function msg = [data, readData](sol::state_view*)
        {
            std::string message(static_cast<char*>(data), readData);
            return message;
        };
        const std::function type = [chatEntryType](sol::state_view*)
        {
            return static_cast<int>(chatEntryType);
        };

        Core::INSTANCE->BroadcastSceneEvent(SCENE_EVENT_LOBBY_CHAT_MSG_RECEIVED, sd, msg, type);
    }

    free(data);
}

void SteamEvents::OnLobbyChatUpdated(LobbyChatUpdate_t* pParam)
{
    SteamIDWrapper user(pParam->m_ulSteamIDUserChanged);

    if ((pParam->m_rgfChatMemberStateChange & EChatMemberStateChange::k_EChatMemberStateChangeEntered) != 0)
    {
        Core::INSTANCE->BroadcastSceneEvent<SteamIDWrapper>(
            SCENE_EVENT_LOBBY_PLAYER_ENTERED, [user](sol::state_view* stateview)
            {
                return user;
            });
    }

    if ((pParam->m_rgfChatMemberStateChange & EChatMemberStateChange::k_EChatMemberStateChangeLeft) != 0 ||
        (pParam->m_rgfChatMemberStateChange & EChatMemberStateChange::k_EChatMemberStateChangeDisconnected) != 0)
    {
        Core::INSTANCE->BroadcastSceneEvent<SteamIDWrapper>(
            SCENE_EVENT_LOBBY_PLAYER_LEFT, [user](sol::state_view* stateview)
            {
                return user;
            });
    }
}

void SteamEvents::OnLobbyEnter(LobbyEnter_t* pParam)
{
    SteamIDWrapper lobby(pParam->m_ulSteamIDLobby);
    uint32 resp = pParam->m_EChatRoomEnterResponse;
    Core::INSTANCE->BroadcastSceneEvent<SteamIDWrapper, uint32>(SCENE_EVENT_ENTERED_LOBBY,
        [lobby](sol::state_view* stateview)
        {
            return lobby;
        },
        [resp](sol::state_view* stateview)
        {
            return resp;
        }
    );

    if (pParam->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
    {
        return;
    }
    
    TraceLog(LOG_INFO, "Steam lobby entered");
    GameLobby::JoinedOrCreated(pParam->m_ulSteamIDLobby);
}

void SteamEvents::OnJoinLobbyRequested(GameLobbyJoinRequested_t* pParam)
{
    SteamIDWrapper lobby(pParam->m_steamIDLobby);
    SteamIDWrapper fr(pParam->m_steamIDFriend);

    Core::INSTANCE->BroadcastSceneEvent<SteamIDWrapper, SteamIDWrapper>(SCENE_EVENT_JOIN_LOBBY_REQUEST,
        [lobby](sol::state_view* stateview)
        {
            return lobby;
        },
        [fr](sol::state_view* stateview)
        {
            return fr;
        }
    );
}
