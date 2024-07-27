#include "LuaMod.h"

#include "lua_hook.h"
#include "raylib.h"
#include "core/Core.h"
#include "ui/Transition.h"
#include "ui/ui_helper.h"

static int CustomRequire(lua_State* lua)
{
    std::string path = sol::stack::get<std::string>(lua, 1);
    std::ranges::replace(path, '.', '/');
    std::string filepath = "resources/scripts/" + path + ".lua";

    if (FileExists(filepath.c_str()))
    {
        luaL_loadfile(lua, filepath.c_str());
        return 1;
    }

    filepath = "mods/" + path + ".lua";
    if (FileExists(filepath.c_str()))
    {
        luaL_loadfile(lua, filepath.c_str());
        return 1;
    }

    return 0;
}

LuaMod::LuaMod(const std::string& filepath, bool privileged) : m_rootDir(GetDirectoryPath(filepath.c_str()))
{
    TraceLog(LOG_INFO, std::format("Loading mod at: {}", m_rootDir).c_str());

    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::bit32, sol::lib::coroutine);
    m_lua.add_package_loader(CustomRequire, false);

    lua_hook::AddCppTypes(&m_lua, privileged);
    lua_hook::AddCppFuncs(&m_lua, privileged);

    sol::protected_function_result result = m_lua.script_file(filepath);
    sol::table modTable = lua_utils::UnwrapResult<sol::table>(result, "Failed to initialize mod at" + filepath);

    name = modTable["name"];
    const std::optional<std::string> desc = modTable["description"];
    description = desc;

    const sol::table event_listeners = modTable.get_or("events", m_lua.create_table());
    m_eventListeners = event_listeners;
}

LuaMod::~LuaMod()
{
    TraceLog(LOG_INFO, std::format("Unloading mod at: {}", m_rootDir).c_str());
}