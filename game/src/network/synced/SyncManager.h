#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "SyncedList.h"
#include "SyncedVar.h"

class SyncManager
{
public:
    SyncManager();
    std::shared_ptr<SyncedVar> NewVar(const std::string& modId, const std::string& id, const sol::object& initialValue, const bool& hostOnly);
    void Update(const std::string& modId, const std::string& id, const sol::object& val, const bool& hostOnly);
    
    std::shared_ptr<SyncedList> NewList(const std::string& modid, const std::string& id, const sol::table& initialValue, const bool& hostOnly);
    std::weak_ptr<SyncedList> GetList(lua_State* L, const std::string& modId, const std::string& id,
                                     const bool& hostOnly);
private:
    std::unordered_map<std::string, std::weak_ptr<SyncedVar>> m_vars;
};
