#include "NetworkManager.h"

#include <isteamnetworkingutils.h>
#include <isteamuser.h>

#include "GameLobby.h"
#include "LuaPacketType.h"
#include "core/Core.h"
#include "lua/LuaConstants.h"
#include "steam/SteamIDWrapper.h"
#include "synced/SyncPacket.h"

NetworkManager::NetworkManager():
    m_messageBuffer(static_cast<SteamNetworkingMessage_t**>(malloc(message_buffer_size * sizeof(SteamNetworkingMessage_t*))))
{
    SteamNetworkingUtils()->InitRelayNetworkAccess();
    m_currentLobbyPlayers.reserve(4);
}

NetworkManager::~NetworkManager()
{
    for (IPacketType* p : m_packets)
    {
        delete p;
    }
    
    free(m_messageBuffer);
}

void NetworkManager::UnregisterPackets()
{
    m_packets.clear();
}

void NetworkManager::RegisterPackets()
{
    m_packets.emplace_back(new SyncPacket);
    Core::INSTANCE->modManager.BroadcastEvent<std::function<uint32_t(const sol::table&)>>(
        GAME_EVENT_ADD_PACKETS, [this](sol::state*)
        {
            return [this](const sol::table& packet)
            {
                const uint32_t idx = static_cast<uint32_t>(m_packets.size());
                TraceLog(LOG_INFO, std::format("Adding custom lua packet type with id {}", idx).c_str());
                this->m_packets.emplace_back(new LuaPacketType(idx, packet));
                return idx;
            };
        });
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

void NetworkManager::JoinLobby(CSteamID lobby)
{
    SteamMatchmaking()->JoinLobby(lobby);
}

void NetworkManager::OnLobbyCreated(LobbyCreated_t* lobby, bool failed)
{
    if (lobby->m_eResult != k_EResultOK || failed || lobby->m_eResult == 0)
    {
        TraceLog(LOG_ERROR, "Failed to create steam lobby");
        return;
    }

    TraceLog(LOG_INFO, "Steam lobby created");
}

void NetworkManager::SendMessage(const NetworkTarget target, const std::optional<SteamIDWrapper>& specifiedTarget,uint32_t type, const sol::object& data, int flags = -1)
{
    if (type < 0 || type > m_packets.size())
    {
        TraceLog(LOG_ERROR, std::format("Invalid packet type {}", type).c_str());
        return;
    }

    auto currentUser = SteamUser()->GetSteamID();

    if (target == EVERYONE)
    {
        m_packets[type]->RawHandle(currentUser, data);
        // send to everyone but only self in lobby
        int n = GameLobby::CURRENT_LOBBY.value()->GetMemberCount();
        if (n == 1 || n == 0)
        {
            return;
        }
    }

    if (flags == -1)
    {
        flags = k_nSteamNetworkingSend_Unreliable;
    }

    m_memWriter.Clear();
    const IPacketType* t = m_packets[type];
    
    /// MESSAGE FORMAT AS FOLLOWS
    /// 4 bytes | Packet Type
    /// 8 bytes | Sender Steam ID
    /// Unknown | Data

    m_memWriter.WriteGeneric<uint32_t>(type);
    m_memWriter.WriteGeneric<uint64>(currentUser.ConvertToUint64());
    t->Serialize(m_memWriter, data);

    std::vector<SteamNetworkingIdentity> identities;
    SetTarget(target, &identities, specifiedTarget);

    for (const SteamNetworkingIdentity& tar : identities)
    {
        if (tar.IsInvalid())
        {
            TraceLog(LOG_ERROR, std::format("Tried to send network packet to invalid user: {} with type: {}",
                                            tar.GetSteamID64(), static_cast<int>(tar.m_eType)).c_str());
            continue;
        }

        SteamNetworkingMessages()->SendMessageToUser(tar, m_memWriter.GetPtr(),
                                                     static_cast<uint32>(m_memWriter.GetSize()), flags, 0);
    }
}

void NetworkManager::HandleMessage(const SteamNetworkingMessage_t* message) const
{
    MemoryReader reader(message->GetData(), message->GetSize());
    uint32_t type = reader.ReadGeneric<uint32_t>();
    CSteamID id = reader.ReadGeneric<uint64>();

    if (type < 0 || type > m_packets.size())
    {
        TraceLog(LOG_ERROR, std::format("Received packet of invalid type: {}", type).c_str());
        return;
    }

    const IPacketType* t = m_packets[type];
    t->Handle(id, reader);
}

void NetworkManager::SetTarget(const NetworkTarget& target, std::vector<SteamNetworkingIdentity>* identities,
                               std::optional<SteamIDWrapper> specifiedTarget)
{
    switch (target)
    {
    case EVERYONE:
    case EVERYONE_ELSE:
        GameLobby::CURRENT_LOBBY.value()->GetAllMembers(&m_currentLobbyPlayers);
        for (const SteamIDWrapper& element : m_currentLobbyPlayers)
        {
            if (element == SteamUser()->GetSteamID()) continue;
            SteamNetworkingIdentity identity;
            identity.SetSteamID(element);
            identities->push_back(identity);
        }
        break;
    case HOST:
        SteamNetworkingIdentity identity;
        identity.SetSteamID(GameLobby::CURRENT_LOBBY.value()->GetHost());
        identities->push_back(identity);
        break;
    case PLAYER:
        SteamNetworkingIdentity ident;
        ident.SetSteamID(specifiedTarget.value());
        identities->push_back(ident);
        break;
    }
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

void NetworkManager::OnSessionRequested(SteamNetworkingMessagesSessionRequest_t* request)
{
    SteamNetworkingMessages()->AcceptSessionWithUser(request->m_identityRemote);
}

void NetworkManager::OnMessageSessionFailed(SteamNetworkingMessagesSessionFailed_t* pParam)
{
    TraceLog(LOG_WARNING, "Failed to establish network session with a user");
}
