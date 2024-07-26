#pragma once
#include "sol/sol.hpp"

namespace lua_hook
{
    void AddCppTypes(sol::state* state, bool privilege);
    void AddCppFuncs(sol::state* state, bool privilege);
}
