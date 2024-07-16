#include "NetworkManager.h"

#include <isteammatchmaking.h>
#include <isteamnetworkingutils.h>

#include "GameLobby.h"
#include "raylib.h"

NetworkManager::NetworkManager()
{
    SteamNetworkingUtils()->InitRelayNetworkAccess();
}

void NetworkManager::HostLobby()
{
    SteamAPICall_t call = SteamMatchmaking()->CreateLobby(ELobbyType::k_ELobbyTypePublic, 8);
    m_lobbyCreatedCR.Set(call, this, &NetworkManager::OnLobbyCreated);
}

void NetworkManager::OnLobbyCreated(LobbyCreated_t* lobby, bool failed)
{
    if (lobby->m_eResult != k_EResultOK || failed)
    {
        TraceLog(LOG_ERROR, "Failed to create steam lobby");
        return;
    }

    GameLobby::Created(lobby->m_eResult);
}

void NetworkManager::HandleMessages()
{
    SteamNetworkingMessage_t* messages[32];
    int msgCount = SteamNetworkingMessages()->ReceiveMessagesOnChannel(0, messages, 32);
    if (msgCount <= 0) return;
    
    for (int i = 0; i < msgCount; i++)
    {
        SteamNetworkingMessage_t* msg = messages[i];
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
