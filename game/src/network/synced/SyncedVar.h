#pragma once
#include <map>

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
    virtual ~SyncedVar() = default;
    
    void Set(const sol::object& val);
    void SetNoNotify(const sol::object& val);
    sol::object Get();
    void Update(const SteamIDWrapper& target);
    uint32_t Subscribe(const sol::protected_function& callback);
    void Unsubscribe(const uint32_t& index);
    void RemoveSubscribers();
protected:
    bool CanEdit();
    std::map<uint32_t, sol::protected_function> m_subscribers;
private:
    static uint32_t m_nextId;
};
