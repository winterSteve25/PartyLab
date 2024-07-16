#include "GameLobby.h"

#include <isteamuser.h>
#include <string>

std::optional<GameLobby*> GameLobby::CURRENT_LOBBY = std::optional<GameLobby*>(std::nullopt);
const char* GameLobby::LOBBY_DATA_KEY_HOST = "host";

CSteamID GameLobby::GetHost()
{
    const char* host = SteamMatchmaking()->GetLobbyData(m_lobbyId, LOBBY_DATA_KEY_HOST);
    uint64 id = std::stoull(host);
    return id;
}

void GameLobby::Created(uint64 lobbyid)
{
    GameLobby* lobby = new GameLobby;
    lobby->m_lobbyId = lobbyid;
    SteamMatchmaking()->SetLobbyData(lobbyid, LOBBY_DATA_KEY_HOST, std::to_string(lobbyid).c_str());
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

