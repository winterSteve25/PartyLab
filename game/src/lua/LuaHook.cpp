#include "LuaHook.h"

#include <memory>

#include "raylib.h"

LuaHook::LuaHook()
{
    m_luaMods.reserve(1);
}

void LuaHook::LoadMods()
{
    FilePathList mods = LoadDirectoryFiles("resources/scripts");
    const std::string base = "/mod.lua";

    for (unsigned int i = 0; i < mods.count; i++)
    {
        std::string dir = mods.paths[i];
        std::string path = dir + base;
        if (!FileExists(path.c_str())) continue;
        m_luaMods.push_back(new LuaMod(path));
    }

    UnloadDirectoryFiles(mods);

    if (!DirectoryExists("mods")) return;
    mods = LoadDirectoryFiles("mods");
    
    for (unsigned int i = 0; i < mods.count; i++)
    {
        std::string dir = mods.paths[i];
        std::string path = dir + base;
        if (!FileExists(path.c_str())) continue;
        m_luaMods.emplace_back(new LuaMod(path));
    }

    UnloadDirectoryFiles(mods);
}

void LuaHook::UnloadMods()
{
    for (LuaMod*& mod : m_luaMods)
    {
        delete mod;
    }
    
    m_luaMods.clear();
}
