#include "SyncedVar.h"

#include "core/Core.h"

SyncedVar::SyncedVar(const std::string& modId, const std::string& id, const sol::object& initialValue, bool hostOnly):
    val(initialValue),
    id(id),
    modId(modId),
    hostOnly(hostOnly)
{
}

void SyncedVar::Set(const sol::object& val)
{
    this->val = val;
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
