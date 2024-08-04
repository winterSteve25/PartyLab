#include "GameLobby.h"

#include <string>
#include <isteammatchmaking.h>

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
        CSteamID id = SteamMatchmaking()->GetLobbyMemberByIndex(m_lobbyId, i);
        members->emplace_back(id);
    }
}

int GameLobby::GetMemberCount() const
{
    return SteamMatchmaking()->GetNumLobbyMembers(m_lobbyId);
}

int GameLobby::GetMemberLimit() const
{
    return SteamMatchmaking()->GetLobbyMemberLimit(m_lobbyId);
}

void GameLobby::SetData(const std::string& key, const std::string& value) const
{
    SteamMatchmaking()->SetLobbyData(m_lobbyId, key.c_str(), value.c_str());
}

std::string GameLobby::GetData(const std::string& key) const
{
    return SteamMatchmaking()->GetLobbyData(m_lobbyId, key.c_str());
}

void GameLobby::JoinedOrCreated(uint64 lobbyid)
{
    GameLobby* lobby = new GameLobby(lobbyid);
    CURRENT_LOBBY = lobby;
}

void GameLobby::Left()
{
    if (!CURRENT_LOBBY.has_value()) return;
    SteamMatchmaking()->LeaveLobby(CURRENT_LOBBY.value()->m_lobbyId);
    delete CURRENT_LOBBY.value();
    CURRENT_LOBBY = std::optional<GameLobby*>(std::nullopt);
}

