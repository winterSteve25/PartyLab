#pragma once
#include <format>
#include <string>

#include "raylib.h"
#include "sol/sol.hpp"

class LuaMod
{
public:
    std::string name;
    std::optional<std::string> description;

    LuaMod(const std::string& file_path);

    template <typename... Args>
    void ReceiveEvent(const std::string& event, const std::function<Args(sol::state*)>&... args)
    {
        TraceLog(LOG_INFO, std::format("Event '{}' was received in mod '{}'", event, name).c_str());
        std::optional<sol::function> handler = m_eventListeners[event];
        if (!handler.has_value()) return;
        handler.value()(args(&m_lua)...);
    }

private:
    sol::state m_lua;
    sol::table m_eventListeners;
    std::string m_rootDir;

    template <typename Func>
    void AddCPPFunc(const std::string& name, Func&& fn)
    {
        m_lua.set_function("cpp_" + name, fn);
    }

    void AddCPPFunctions();
    void AddCPPTypes();
};
