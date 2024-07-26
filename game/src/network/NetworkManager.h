#pragma once
#include <isteammatchmaking.h>
#include <isteamnetworkingmessages.h>

class NetworkManager
{
public:
    NetworkManager();

    void HostLobby();
private:
    static constexpr uint16_t message_buffer_size = 64;
    
    SteamNetworkingMessage_t* m_messageBuffer[message_buffer_size];
    
    void HandleMessage(const SteamNetworkingMessage_t* message);
    void HandleMessages();
    void SendMessage();
    
    STEAM_CALLBACK(NetworkManager, OnSessionRequested, SteamNetworkingMessagesSessionRequest_t);

    void OnLobbyCreated(LobbyCreated_t* lobby, bool failure);
    CCallResult<NetworkManager, LobbyCreated_t> m_lobbyCreatedCR;
};
