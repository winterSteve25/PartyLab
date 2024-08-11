#pragma once
#include <map>

#include "LuaMod.h"

/**
 * Manages all the lua mods
 */
class ModManager
{
public:

    ModManager();
    
    void LoadMods();
    void UnloadMods();
    LuaMod* GetModWithIdNullable(const std::string& id);
    void TriggerGC() const;
    
    template <typename... Args>
    void BroadcastEvent(const std::string& event, const std::function<Args(sol::state*)>&... args)
    {
        for (auto pair : m_luaMods)
        {
            pair.second->ReceiveEvent<Args...>(event, args...);
        }
    }

private:
    std::map<std::string, LuaMod*> m_luaMods;
};
