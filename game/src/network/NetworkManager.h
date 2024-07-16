#pragma once
#include <isteammatchmaking.h>
#include <isteamnetworkingmessages.h>

class NetworkManager
{
public:
    NetworkManager();

    void HostLobby();
private:
    void HandleMessages();
    void SendMessage();
    
    STEAM_CALLBACK(NetworkManager, OnSessionRequested, SteamNetworkingMessagesSessionRequest_t);

    void OnLobbyCreated(LobbyCreated_t* lobby, bool failure);
    CCallResult<NetworkManager, LobbyCreated_t> m_lobbyCreatedCR;
};
