#include "LuaMod.h"
#include "LuaConstants.h"
#include "raylib.h"
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

    sol::stack::push(lua, std::string("\n\tno file: ") + GetWorkingDirectory() + "/" + filepath);
    return 1;
}

LuaMod::LuaMod(const std::string& filepath) : m_rootDir(GetDirectoryPath(filepath.c_str()))
{
    m_lua.open_libraries(sol::lib::base, sol::lib::package);
    m_lua.add_package_loader(CustomRequire, false);
    AddCPPFunctions();
    AddCPPTypes();

    sol::protected_function_result result = m_lua.script_file(filepath);
    if (!result.valid())
    {
        TraceLog(LOG_ERROR, ("Failed to initialize mod at" + filepath).c_str());
        sol::error err = result;
        TraceLog(LOG_ERROR, ("Status: " + sol::to_string(result.status())).c_str());
        TraceLog(LOG_ERROR, ("Reason: " + std::string(err.what())).c_str());
    }

    sol::table modTable = result;
    name = modTable[GAME_MOD_PROP_NAME];
    const std::optional<std::string> desc = modTable[GAME_MOD_PROP_DESC];
    description = desc;

    const sol::table event_listeners = modTable.get_or(GAME_MOD_PROP_EVENT_HANDLERS, m_lua.create_table());
    m_eventListeners = event_listeners;

    sol::optional<sol::function> onLoad = m_eventListeners[GAME_EVENT_LOAD];
    if (onLoad.has_value()) onLoad.value().call<void>();
}

void LuaMod::AddCPPFunctions()
{
    AddCPPFunc("info", [](const char* text) { TraceLog(LOG_INFO, text); });
    AddCPPFunc("warning", [](const char* text) { TraceLog(LOG_WARNING, text); });
    AddCPPFunc("error", [](const char* text) { TraceLog(LOG_ERROR, text); });

    AddCPPFunc("getScreenWidth", GetScreenWidth);
    AddCPPFunc("getScreenHeight", GetScreenHeight);

    AddCPPFunc("anchorTopLeft", ui_helper::AnchorTopLeft);
    AddCPPFunc("anchorTopRight", ui_helper::AnchorTopRight);
    AddCPPFunc("anchorBtmLeft", ui_helper::AnchorBtmLeft);
    AddCPPFunc("anchorBtmRight", ui_helper::AnchorBtmRight);
    AddCPPFunc("drawText", ui_helper::DrawText);
    AddCPPFunc("measureText", ui_helper::MeasureText);
    AddCPPFunc("getCenter", ui_helper::GetCenter);
    AddCPPFunc("within", ui_helper::Within);
}

void LuaMod::AddCPPTypes()
{
    m_lua.new_usertype<Vector2>(
        "Vector2",
        sol::call_constructor,
        sol::factories([](const double& x, const double& y)
        {
            return std::make_shared<Vector2>(x, y);
        }),
        "x", &Vector2::x,
        "y", &Vector2::y
    );

    m_lua.new_usertype<Color>(
        "Color",
        sol::call_constructor,
        sol::factories([](const double& r, const double& g, const double& b, const double& a)
        {
            return std::make_shared<Color>(r, g, b, a);
        }),
        "r", &Color::r,
        "g", &Color::g,
        "b", &Color::b,
        "a", &Color::a
    );
}
