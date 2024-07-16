#pragma once
#include <string>
#include "sol/sol.hpp"

class LuaMod
{
public:
    std::string name;
    std::optional<std::string> description;

    LuaMod(const std::string& file_path);
private:
    sol::state m_lua;
    sol::table m_eventListeners;
};
