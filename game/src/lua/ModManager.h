#pragma once
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
    
    template <typename... Args>
    void BroadcastEvent(const std::string& event, const std::function<Args(sol::state*)>&... args)
    {
        for (auto pair : m_luaMods)
        {
            pair.second->ReceiveEvent<Args...>(event, args...);
        }
    }

private:
    std::unordered_map<std::string, LuaMod*> m_luaMods;
};
