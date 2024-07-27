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
    
    template <typename... Args>
    void BroadcastEvent(const std::string& event, const std::function<Args(sol::state*)>&... args)
    {
        for (LuaMod*& lua : m_luaMods)
        {
            lua->ReceiveEvent<Args...>(event, args...);
        }
    }

private:
    std::vector<LuaMod*> m_luaMods;
};
