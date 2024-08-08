#include "SyncManager.h"

#include <format>
#include <memory>

#include "raylib.h"

SyncManager::SyncManager() = default;

std::shared_ptr<SyncedVar> SyncManager::NewVar(const std::string& modId, const std::string& id, const sol::object& initialValue, const bool& hostOnly)
{
    auto realID = std::format("{}:{}", modId, id);

    if (m_vars.contains(realID) && !m_vars[realID].expired())
    {
        TraceLog(LOG_WARNING, std::format("Tried create a synced var of id {}, but one already exists, updating its value instead", realID).c_str());
        Update(modId, id, initialValue, hostOnly);
        return m_vars[realID].lock();
    }

    auto var = std::make_shared<SyncedVar>(modId, id, initialValue, hostOnly);
    m_vars[realID] = var;
    return var;
}

void SyncManager::Update(const std::string& modId, const std::string& id, const sol::object& val, const bool& hostOnly)
{
    auto realID = std::format("{}:{}", modId, id);
    
    if (!m_vars.contains(realID))
    {
        TraceLog(LOG_WARNING, std::format("Tried updating a non-existent synced var of id {}, creating it instead", realID).c_str());
        NewVar(modId, id, val, hostOnly); 
        return;
    }

    if (const std::shared_ptr<SyncedVar> var = m_vars[realID].lock())
    {
        var->val = val;
    } else
    {
        TraceLog(LOG_WARNING, std::format("Tried updating a already deleted synced var of id {}, ignoring update call", realID).c_str());
        m_vars.erase(realID);
    }
}
