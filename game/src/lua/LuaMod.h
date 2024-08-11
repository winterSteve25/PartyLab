#pragma once
#include <format>
#include <string>

#include "lua_utils.h"
#include "raylib.h"
#include "sol/sol.hpp"

/**
 * A lua mod container, contains the lua state used to execute all the lua code within this mod
 */
class LuaMod
{
public:
    std::string name;
    std::optional<std::string> description;

    LuaMod(const std::string& file_path, bool privileged, const std::string& modId);
    ~LuaMod();

    const sol::state& GetStateRef() const;
    sol::state* GetState();
    
    template <typename... Args>
    void ReceiveEvent(const std::string& event, const std::function<Args(sol::state*)>&... args)
    {
        TraceLog(LOG_INFO, std::format("Event '{}' was received in mod '{}'", event, name).c_str());
        std::optional<sol::protected_function> handler = m_eventListeners[event];
        if (!handler.has_value()) return;
        sol::protected_function_result result = handler.value()(args(&m_lua)...);
        lua_utils::UnwrapResult(result, "Failed to run event handler");
    }

private:
    sol::state m_lua;
    sol::table m_eventListeners;
    std::string m_rootDir;
};
