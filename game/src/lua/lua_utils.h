#pragma once

#include "raylib.h"
#include "sol/sol.hpp"

namespace lua_utils
{
    template <typename T>
    static T UnwrapResult(const sol::protected_function_result& result, const std::string& msg)
    {
        if (!result.valid())
        {
            TraceLog(LOG_ERROR, msg.c_str());
            sol::error err = result;
            TraceLog(LOG_ERROR, ("Status: " + sol::to_string(result.status())).c_str());
            TraceLog(LOG_ERROR, ("Reason: " + std::string(err.what())).c_str());
            std::terminate();
        }

        T r = result;
        return r;
    }

    static void UnwrapResult(const sol::protected_function_result& result, const std::string& msg)
    {
        if (result.valid()) return;
        TraceLog(LOG_ERROR, msg.c_str());
        sol::error err = result;
        TraceLog(LOG_ERROR, ("Status: " + sol::to_string(result.status())).c_str());
        TraceLog(LOG_ERROR, ("Reason: " + std::string(err.what())).c_str());
    }
}
