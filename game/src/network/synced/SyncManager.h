#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "SyncedVar.h"

class SyncManager
{
public:
    SyncManager();
    std::shared_ptr<SyncedVar> NewVar(const std::string& modId, const std::string& id, const sol::object& initialValue, const bool& hostOnly);
    void Update(const std::string& modId, const std::string& id, const sol::object& val, const bool& hostOnly);
private:
    std::unordered_map<std::string, std::weak_ptr<SyncedVar>> m_vars;
};
