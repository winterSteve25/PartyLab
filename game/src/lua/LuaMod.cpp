#include "LuaMod.h"

LuaMod::LuaMod(const std::string& filepath)
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math);
    sol::table result = m_lua.script_file(filepath);

    name = result["name"];
    std::optional<std::string> descr = result["description"];
    description = descr;
    sol::table eventListeners = result["events"];
    m_eventListeners = eventListeners;
}

