#pragma once
#include <optional>
#include <steamtypes.h>

#include "NetworkManager.h"

class GameLobby
{
public:
    static std::optional<GameLobby*> CURRENT_LOBBY;
    static const char* LOBBY_DATA_KEY_HOST;

    GameLobby() = default;
    CSteamID GetHost() const;
private:
    CSteamID m_lobbyId;
    
    static void Created(uint64 lobbyid);
    static void Joined(uint64 lobbyid);
    static void Left();
    
    friend NetworkManager;
};
