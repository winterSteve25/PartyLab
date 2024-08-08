#include "SyncedVar.h"

#include "core/Core.h"

uint32_t SyncedVar::m_nextId = 0;

SyncedVar::SyncedVar(const std::string& modId, const std::string& id, const sol::object& initialValue, bool hostOnly):
    val(initialValue),
    id(id),
    modId(modId),
    hostOnly(hostOnly)
{
}

void SyncedVar::Set(const sol::object& val)
{
    SetNoNotify(val);
    sol::table table = sol::table::create(val.lua_state());
    table["obj"] = this;
    
    Core::INSTANCE->networkManager.SendMessage(
        EVERYONE_ELSE,
        std::optional<SteamIDWrapper>(std::nullopt),
        0,
        table,
        k_nSteamNetworkingSend_Reliable
    );
}

void SyncedVar::SetNoNotify(const sol::object& val)
{
    this->val = val;
    
    for (auto pairs : m_subscribers)
    {
        lua_utils::UnwrapResult(pairs.second(val), std::format("Failed to call synced var subscriber for synced var with id {}:{}", modId, id));
    }
}

sol::object SyncedVar::Get()
{
    return val;
}

void SyncedVar::Update(const SteamIDWrapper& target)
{
    sol::table table = sol::table::create(val.lua_state());
    table["obj"] = this;
    
    Core::INSTANCE->networkManager.SendMessage(
        PLAYER,
        target,
        0,
        table,
        k_nSteamNetworkingSend_Reliable
    );
}

uint32_t SyncedVar::Subscribe(const sol::protected_function& callback)
{
    uint32_t idx = m_nextId;
    m_subscribers[idx] = callback;
    m_nextId++;
    return idx;
}

void SyncedVar::Unsubscribe(const uint32_t& index)
{
    m_subscribers.erase(index);
}
