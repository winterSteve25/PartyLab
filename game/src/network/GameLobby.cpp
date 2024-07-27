#include "GameLobby.h"

#include <isteamclient.h>
#include <string>

std::optional<GameLobby*> GameLobby::CURRENT_LOBBY = std::optional<GameLobby*>(std::nullopt);

CSteamID GameLobby::GetHost() const
{
    return SteamMatchmaking()->GetLobbyOwner(m_lobbyId);
}

void GameLobby::GetAllMembers(std::vector<CSteamID>* members) const
{
    members->clear();
    
    int n = SteamMatchmaking()->GetNumLobbyMembers(m_lobbyId);
    for (int i = 0; i < n; i++)
    {
        members->push_back(SteamMatchmaking()->GetLobbyMemberByIndex(m_lobbyId, i));
    }
}

void GameLobby::Created(uint64 lobbyid)
{
    GameLobby* lobby = new GameLobby;
    lobby->m_lobbyId = lobbyid;
    CURRENT_LOBBY = lobby;
}

void GameLobby::Joined(uint64 lobbyid)
{
}

void GameLobby::Left()
{
    if (!CURRENT_LOBBY.has_value()) return;
    delete CURRENT_LOBBY.value();
    CURRENT_LOBBY = std::optional<GameLobby*>(std::nullopt);
}

