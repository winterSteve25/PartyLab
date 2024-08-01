#include "NetworkManager.h"

#include <isteammatchmaking.h>
#include <isteamnetworkingutils.h>

#include "GameLobby.h"
#include "raylib.h"

NetworkManager::NetworkManager():
    m_messageBuffer(new SteamNetworkingMessage_t[message_buffer_size])
{
    SteamNetworkingUtils()->InitRelayNetworkAccess();
}

void NetworkManager::HostLobby()
{
    SteamAPICall_t call = SteamMatchmaking()->CreateLobby(ELobbyType::k_ELobbyTypePublic, 8);
    m_lobbyCreatedCR.Set(call, this, &NetworkManager::OnLobbyCreated);
}

void NetworkManager::LeaveCurrentLobby()
{
    TraceLog(LOG_INFO, "Left current lobby");
    GameLobby::Left();
}

void NetworkManager::OnLobbyCreated(LobbyCreated_t* lobby, bool failed)
{
    if (lobby->m_eResult != k_EResultOK || failed || lobby->m_eResult == 0)
    {
        TraceLog(LOG_ERROR, "Failed to create steam lobby");
        return;
    }

    TraceLog(LOG_INFO, "Steam lobby created");
    GameLobby::Created(lobby->m_eResult);
    GameLobby* gameLobby = GameLobby::CURRENT_LOBBY.value();
    gameLobby->SetData("hello", "hello");
}

void NetworkManager::HandleMessage(const SteamNetworkingMessage_t* message)
{
}

void NetworkManager::HandleMessages()
{
    // TODO: investigate this, this call blocks forever sometimes 
    int msgCount = SteamNetworkingMessages()->ReceiveMessagesOnChannel(0, m_messageBuffer, message_buffer_size);
    if (msgCount <= 0) return;

    for (int i = 0; i < msgCount; i++)
    {
        SteamNetworkingMessage_t* msg = m_messageBuffer[i];
        HandleMessage(msg);
        msg->Release();
    }
}

void NetworkManager::SendMessage()
{
}

void NetworkManager::OnSessionRequested(SteamNetworkingMessagesSessionRequest_t* request)
{
    SteamNetworkingMessages()->AcceptSessionWithUser(request->m_identityRemote);
}
