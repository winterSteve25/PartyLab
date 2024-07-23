#include "LuaMod.h"
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

LuaMod::LuaMod(const std::string& filepath) : m_rootDir(GetDirectoryPath(filepath.c_str()))
{
    TraceLog(LOG_INFO, std::format("Loading mod at: {}", m_rootDir).c_str());
    
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::bit32);
    m_lua.add_package_loader(CustomRequire, false);

    AddCPPFunctions();
    AddCPPTypes();

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

void LuaMod::AddCPPFunctions()
{
    AddCPPFunc("info", [](const char* text) { TraceLog(LOG_INFO, text); });
    AddCPPFunc("warning", [](const char* text) { TraceLog(LOG_WARNING, text); });
    AddCPPFunc("error", [](const char* text) { TraceLog(LOG_ERROR, text); });

    AddCPPFunc("getScreenWidth", GetScreenWidth);
    AddCPPFunc("getScreenHeight", GetScreenHeight);

    AddCPPFunc("measureText", ui_helper::MeasureText);
    AddCPPFunc("getCenter", ui_helper::GetCenter);
    AddCPPFunc("within", ui_helper::Within);

    AddCPPFunc("transitionTo", [](int scene) { Core::INSTANCE->transitionManager.TransitionTo(scene); });
    AddCPPFunc("exit", []() { Core::INSTANCE->shouldExit = true; });
}

void LuaMod::AddCPPTypes()
{
    m_lua.new_usertype<Vector2>(
        "Vector2",
        sol::call_constructor,
        sol::factories(
            [](const double& x, const double& y)
            {
                return std::make_shared<Vector2>(x, y);
            },
            []()
            {
                constexpr Vector2 v{0, 0};
                return v;
            }
        ),
        "x", &Vector2::x,
        "y", &Vector2::y,

        "dot",
        [](const Vector2& self, Vector2& other)
        {
            return self.x * other.x + self.y * other.y;
        },

        sol::meta_function::addition,
        [](const Vector2& a, const Vector2& b)
        {
            Vector2 r = {a.x + b.x, a.y + b.y};
            return r;
        },

        sol::meta_function::subtraction,
        [](const Vector2& a, const Vector2& b)
        {
            Vector2 r = {a.x - b.x, a.y - b.y};
            return r;
        },

        sol::meta_function::equal_to,
        [](const Vector2& a, const Vector2& b)
        {
            return a.x == b.x && a.y == b.y;
        }
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

    m_lua.new_usertype<Transition>(
        "Transition",
        sol::call_constructor,
        sol::factories(
            [](const unsigned int& duration, const sol::object& ease)
            {
                return std::make_shared<Transition>(duration, ease);
            }
        ),
        "duration",
        &Transition::duration,
        "ease",
        &Transition::ease
    );
}
