#pragma once
#include "sol/sol.hpp"
#include "steam/SteamIDWrapper.h"

class SyncedVar
{
public:
    sol::object val;
    std::string id;
    std::string modId;
    bool hostOnly;
    
    SyncedVar(const std::string& modId, const std::string& id, const sol::object& initialValue, bool hostOnly);
    void Set(const sol::object& val);
    sol::object Get();
    void Update(const SteamIDWrapper& target);
};
