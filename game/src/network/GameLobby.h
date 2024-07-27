#pragma once
#include <optional>
#include <steamtypes.h>
#include <vector>

#include "NetworkManager.h"

/**
 * Wrapper around a raw steam lobby, which is just represented by a CSteamID, which is a 64bit integer
 * Provides helper functions related to a specific lobby instance
 */
class GameLobby
{
public:
    static std::optional<GameLobby*> CURRENT_LOBBY;

    GameLobby() = default;
    
    CSteamID GetHost() const;
    void GetAllMembers(std::vector<CSteamID>* members) const;
    
private:
    CSteamID m_lobbyId;
    
    static void Created(uint64 lobbyid);
    static void Joined(uint64 lobbyid);
    static void Left();
    
    friend NetworkManager;
};
