#include "SyncedList.h"

#include "core/Core.h"

uint32_t SyncedList::m_nextAddId = 0;
uint32_t SyncedList::m_nextRemoveId = 0;

SyncedList::SyncedList(const std::string& modId, const std::string& id, const sol::table& initialValue, bool hostOnly):
    SyncedVar(modId, id, initialValue, hostOnly),
    m_len(0)
{
    UpdateLen();
}

void SyncedList::Add(const sol::object& val)
{
    if (!CanEdit()) return;

    AddNoNotify(val);
    sol::table table = sol::table::create(val.lua_state());

    table["obj"] = this;
    table["op"] = 0;
    table["val"] = val;

    Core::INSTANCE->networkManager.SendMessage(
        EVERYONE_ELSE,
        std::optional<SteamIDWrapper>(std::nullopt),
        1,
        table,
        k_nSteamNetworkingSend_Reliable
    );
}

void SyncedList::AddNoNotify(const sol::object& val)
{
    sol::table table = this->val.as<sol::table>();
    table.add(val);

    for (auto pairs : m_addSubscribers)
    {
        lua_utils::UnwrapResult(pairs.second(m_len, val),
                                std::format("Failed to call synced var subscriber for synced var with id {}:{}", modId,
                                            id));
    }
    
    m_len++;
}

void SyncedList::Remove(const int& index)
{
    if (!CanEdit()) return;

    RemoveNoNotify(index);
    sol::table table = sol::table::create(val.lua_state());

    table["obj"] = this;
    table["op"] = 1;
    table["val"] = index;

    Core::INSTANCE->networkManager.SendMessage(
        EVERYONE_ELSE,
        std::optional<SteamIDWrapper>(std::nullopt),
        1,
        table,
        k_nSteamNetworkingSend_Reliable
    );
}

void SyncedList::RemoveNoNotify(const int& index)
{
    auto table = val.as<sol::table>();

    for (auto pairs : m_removeSubscribers)
    {
        lua_utils::UnwrapResult(pairs.second(index, table[index]),
                                std::format("Failed to call synced var subscriber for synced var with id {}:{}", modId,
                                            id));
    }

    table[index] = sol::lua_nil;
    m_len--;
}

uint32_t SyncedList::SubscribeAdd(const sol::protected_function& callback)
{
    uint32_t idx = m_nextAddId;
    m_addSubscribers[idx] = callback;
    m_nextAddId++;
    return idx;
}

void SyncedList::UnsubscribeAdd(const uint32_t& index)
{
    m_addSubscribers.erase(index);
}

void SyncedList::RemoveAddSubscribers()
{
    m_addSubscribers.clear();
}

uint32_t SyncedList::SubscribeRemove(const sol::protected_function& callback)
{
    uint32_t idx = m_nextRemoveId;
    m_removeSubscribers[idx] = callback;
    m_nextRemoveId++;
    return idx;
}

void SyncedList::UnsubscribeRemove(const uint32_t& index)
{
    m_removeSubscribers.erase(index);
}

void SyncedList::RemoveRemoveSubscribers()
{
    m_removeSubscribers.clear();
}

void SyncedList::SetNoNotify(const sol::object& val)
{
    SyncedVar::SetNoNotify(val);
    UpdateLen();
}

void SyncedList::UpdateLen()
{
    m_len = 0;
    uint32_t* ptr = &m_len;
    
    val.as<sol::table>().for_each([ptr](auto pair)
    {
        if (pair.second == sol::lua_nil) return;
        (*ptr)++;
    });
}
