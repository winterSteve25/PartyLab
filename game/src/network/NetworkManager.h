#pragma once
#include <isteammatchmaking.h>
#include <isteamnetworkingmessages.h>

#include "NetworkTarget.h"
#include "IPacketType.h"
#include "steam/SteamIDWrapper.h"

/**
 * Manages the network messages, does not store connections b/c of how new steam networking api works
 */
class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    void UnregisterPackets();
    void RegisterPackets();
    
    void HandleMessages();
    void HostLobby();
    void LeaveCurrentLobby();
    void JoinLobby(CSteamID lobby);
    void SendMessage(NetworkTarget target, const std::optional<SteamIDWrapper>& specifiedTarget, uint32_t type, const sol::object& data, int flags);

private:
    static constexpr uint16_t message_buffer_size = 64;

    MemoryWriter m_memWriter;
    std::vector<IPacketType*> m_packets;
    // not guaranteed to be up-to-date, before using should call GameLobby::getAllMembers
    std::vector<SteamIDWrapper> m_currentLobbyPlayers;
    SteamNetworkingMessage_t** m_messageBuffer;

    void HandleMessage(const SteamNetworkingMessage_t* message) const;
    void SetTarget(const NetworkTarget& target, std::vector<SteamNetworkingIdentity>* identity, std::optional<SteamIDWrapper> specifiedTarget);

    void OnLobbyCreated(LobbyCreated_t* lobby, bool failure);
    CCallResult<NetworkManager, LobbyCreated_t> m_lobbyCreatedCR;

    STEAM_CALLBACK(NetworkManager, OnSessionRequested, SteamNetworkingMessagesSessionRequest_t);
    STEAM_CALLBACK(NetworkManager, OnMessageSessionFailed, SteamNetworkingMessagesSessionFailed_t);
};
