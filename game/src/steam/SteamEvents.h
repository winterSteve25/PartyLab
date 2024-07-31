#pragma once
#include <isteamfriends.h>
#include <steam_api_common.h>

#include "network/GameLobby.h"

class SteamEvents
{
public:
    STEAM_CALLBACK(SteamEvents, OnAvatarLoaded, AvatarImageLoaded_t);
    STEAM_CALLBACK(SteamEvents, OnLobbyChatMessageReceived, LobbyChatMsg_t);
    STEAM_CALLBACK(SteamEvents, OnLobbyChatUpdated, LobbyChatUpdate_t);
    STEAM_CALLBACK(SteamEvents, OnLobbyEnter, LobbyEnter_t);
};
