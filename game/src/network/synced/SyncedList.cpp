#include "SyncedList.h"

#include "core/Core.h"

SyncedList::SyncedList(const std::string& modId, const std::string& id, const sol::table& initialValue, bool hostOnly):
    SyncedVar(modId, id, initialValue, hostOnly)
{
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
    this->val.as<sol::table>().add(val);

    for (auto pairs : m_subscribers)
    {
        lua_utils::UnwrapResult(pairs.second(this->val), std::format("Failed to call synced var subscriber for synced var with id {}:{}", modId, id));
    }
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
    val.as<sol::table>()[index] = sol::lua_nil;
}