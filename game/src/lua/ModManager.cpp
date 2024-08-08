#include "ModManager.h"

#include <filesystem>
#include <memory>

#include "raylib.h"

ModManager::ModManager()
{
    m_luaMods.reserve(1);
}

void ModManager::LoadMods()
{
    FilePathList mods = LoadDirectoryFiles("resources/scripts");
    const std::filesystem::path base = "mod.lua";

    for (unsigned int i = 0; i < mods.count; i++)
    {
        std::filesystem::path dir = mods.paths[i];
        std::string filename = dir.filename().generic_string();
        std::string path = (dir / base).generic_string();
        if (!FileExists(path.c_str())) continue;
        m_luaMods[filename] = new LuaMod(path, true, filename);
    }

    UnloadDirectoryFiles(mods);

    if (!DirectoryExists("mods")) return;
    mods = LoadDirectoryFiles("mods");
    
    for (unsigned int i = 0; i < mods.count; i++)
    {
        std::filesystem::path dir = mods.paths[i];
        std::string filename = dir.filename().generic_string();
        if (m_luaMods.contains(filename) || filename == "partylab")
        {
            TraceLog(LOG_WARNING, std::format("Skipping mod: {}, mod with same id already exists", filename).c_str());
            continue;
        }
        std::string path = (dir / base).generic_string();
        if (!FileExists(path.c_str())) continue;
        m_luaMods[filename] = new LuaMod(path, true, filename);
    }

    UnloadDirectoryFiles(mods);
}

void ModManager::UnloadMods()
{
    for (auto pair : m_luaMods)
    {
        delete pair.second;
    }
    
    m_luaMods.clear();
}

LuaMod* ModManager::GetModWithIdNullable(const std::string& id)
{
    if (!m_luaMods.contains(id))
    {
        return nullptr;
    }

    return m_luaMods[id];
}
