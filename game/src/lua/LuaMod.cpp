#include "LuaMod.h"

#include <filesystem>

#include "raylib.h"
#include "hooks/lua_hook.h"
#include "ui/Transition.h"

static int CustomRequire(lua_State* lua)
{
    std::string path = sol::stack::get<std::string>(lua, 1);
    std::ranges::replace(path, '.', '/');
    std::filesystem::path actualPath = std::filesystem::path(path + ".lua");

    const std::filesystem::path resourcesDir = "resources/scripts";
    std::filesystem::path modsDir = "mods";
    std::filesystem::path filepath = resourcesDir / actualPath;
    std::string filepathInStr = filepath.generic_string();

    if (FileExists(filepathInStr.c_str()))
    {
        luaL_loadfile(lua, filepathInStr.c_str());
        return 1;
    }

    filepath = modsDir / actualPath;
    filepathInStr = filepath.generic_string();

    if (FileExists(filepathInStr.c_str()))
    {
        luaL_loadfile(lua, filepathInStr.c_str());
        return 1;
    }

    return 0;
}

LuaMod::LuaMod(const std::string& filepath, bool privileged) : m_rootDir(GetDirectoryPath(filepath.c_str()))
{
    TraceLog(LOG_INFO, std::format("Loading mod at: {}", m_rootDir).c_str());

    m_lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::bit32,
        sol::lib::coroutine,
        sol::lib::string
    );
    m_lua.add_package_loader(CustomRequire, false);

    lua_hook::AddCppTypes(&m_lua, privileged);
    lua_hook::AddCppFuncs(&m_lua, privileged, m_rootDir);

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
