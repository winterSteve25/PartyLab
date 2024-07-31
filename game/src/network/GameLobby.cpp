#include "GameLobby.h"

#include <isteamclient.h>
#include <string>

#include "raylib.h"
#include "steam/SteamIDWrapper.h"

std::optional<GameLobby*> GameLobby::CURRENT_LOBBY = std::optional<GameLobby*>(std::nullopt);

GameLobby::GameLobby(CSteamID id): m_lobbyId(id)
{
}

CSteamID GameLobby::GetHost() const
{
    return SteamMatchmaking()->GetLobbyOwner(m_lobbyId);
}

void GameLobby::SendChatString(const std::string& message) const
{
    bool result = SteamMatchmaking()->SendLobbyChatMsg(m_lobbyId, message.c_str(), strlen(message.c_str()) + 1);
    if (!result)
    {
        TraceLog(LOG_WARNING, "Failed to send message to lobby chat");
    }
}

void GameLobby::GetAllMembers(std::vector<SteamIDWrapper>* members) const
{
    members->clear();
    
    int n = SteamMatchmaking()->GetNumLobbyMembers(m_lobbyId);
    for (int i = 0; i < n; i++)
    {
        members->emplace_back(SteamMatchmaking()->GetLobbyMemberByIndex(m_lobbyId, i));
    }
}

void GameLobby::SetData(const std::string& key, const std::string& value) const
{
    SteamMatchmaking()->SetLobbyData(m_lobbyId, key.c_str(), value.c_str());
}

void GameLobby::Created(uint64 lobbyid)
{
    GameLobby* lobby = new GameLobby(lobbyid);
    CURRENT_LOBBY = lobby;
}

void GameLobby::Joined(uint64 lobbyid)
{
}

void GameLobby::Left()
{
    if (!CURRENT_LOBBY.has_value()) return;
    SteamMatchmaking()->LeaveLobby(CURRENT_LOBBY.value()->m_lobbyId);
    delete CURRENT_LOBBY.value();
    CURRENT_LOBBY = std::optional<GameLobby*>(std::nullopt);
}

