#pragma once

#include <filesystem>
#include "sol/sol.hpp"

namespace lua_steam_hook
{
    void AddCppTypes(sol::state* state, bool privilege);
    void AddCppFuncs(sol::state* state, bool privilege, const std::filesystem::path& modDir);
}
