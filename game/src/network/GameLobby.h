#pragma once
#include <optional>
#include <steamtypes.h>
#include <string>
#include <vector>

#include "NetworkManager.h"

class SteamIDWrapper;
/**
 * Wrapper around a raw steam lobby, which is just represented by a CSteamID, which is a 64bit integer
 * Provides helper functions related to a specific lobby instance
 */
class GameLobby
{
public:
    static std::optional<GameLobby*> CURRENT_LOBBY;
    GameLobby(CSteamID id);
    
    CSteamID GetHost() const;
    void SendChatString(const std::string& message) const;
    void GetAllMembers(std::vector<SteamIDWrapper>* members) const;
    void SetData(const std::string& key, const std::string& value) const;
private:
    CSteamID m_lobbyId;
    
    static void Created(uint64 lobbyid);
    static void Joined(uint64 lobbyid);
    static void Left();
    
    friend NetworkManager;
};
