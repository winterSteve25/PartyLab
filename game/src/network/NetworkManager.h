#pragma once
#include <isteammatchmaking.h>
#include <isteamnetworkingmessages.h>

/**
 * Manages the network messages, does not store connections b/c of how new steam networking api works
 */
class NetworkManager
{
public:
    NetworkManager();

    void HandleMessages();
    void HostLobby();
    void LeaveCurrentLobby();
private:
    static constexpr uint16_t message_buffer_size = 64;
    
    SteamNetworkingMessage_t* m_messageBuffer[message_buffer_size];
    
    void HandleMessage(const SteamNetworkingMessage_t* message);
    void SendMessage();
    
    void OnLobbyCreated(LobbyCreated_t* lobby, bool failure);
    CCallResult<NetworkManager, LobbyCreated_t> m_lobbyCreatedCR;
    
    STEAM_CALLBACK(NetworkManager, OnSessionRequested, SteamNetworkingMessagesSessionRequest_t);
};
