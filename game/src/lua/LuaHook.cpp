#include "LuaHook.h"
#include "raylib.h"

std::optional<std::shared_ptr<LuaMod>> LuaHook::LoadMod(const std::string& name)
{
    const char* modFile = ("resources/scripts/" + name + "/mod.lua").c_str();
    if (DirectoryExists(("resources/scripts/" + name).c_str()) && FileExists(modFile))
    {
        return std::make_shared<LuaMod>(modFile);
    }


    modFile = ("mods/" + name + "/mod.lua").c_str();
    if (DirectoryExists(("mods/" + name).c_str()) && FileExists(modFile))
    {
        return std::make_shared<LuaMod>(modFile);
    }

    return std::optional<std::shared_ptr<LuaMod>>(std::nullopt);
}
