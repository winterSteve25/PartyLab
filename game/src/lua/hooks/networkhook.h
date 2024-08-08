#pragma once

#include <filesystem>
#include "sol/sol.hpp"

namespace lua_network_hook
{
    void AddCppTypes(sol::state* state, bool privilege, const std::string& modId);
    void AddCppFuncs(sol::state* state, bool privilege, const std::filesystem::path& modDir);
}