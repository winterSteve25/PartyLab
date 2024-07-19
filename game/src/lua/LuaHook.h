#pragma once
#include "LuaMod.h"

class LuaHook
{
public:

    LuaHook();
    void LoadMods();

    template <typename... Args>
    void BroadcastEvent(const std::string& event, const std::function<Args(sol::state*)>&... args)
    {
        for (auto lua : m_luaMods)
        {
            lua->ReceiveEvent<Args...>(event, args...);
        }
    }

private:
    std::vector<std::shared_ptr<LuaMod>> m_luaMods;
};
