#pragma once
#include <vector>

#include "LuaWaitForEvent.h"
#include "lua/lua_utils.h"
#include "sol/sol.hpp"

class LuaAsyncManager
{
private:
    using AwaiterType = std::tuple<
        sol::thread,
        sol::coroutine,
        std::function<void(const sol::protected_function_result&)>,
        sol::optional<sol::protected_function>
    >;

    std::unordered_map<LuaWaitForEvent, std::vector<AwaiterType>> m_awaiters;


    void ContinueCoroutine(const sol::thread& thread, sol::coroutine& coroutine,
                           const std::function<void(const sol::protected_function_result&)>& callback);

public:
    void CallAsync(const sol::coroutine& coroutine,
                   const std::function<void(const sol::protected_function_result&)>& callback);

    template <typename Arg>
    void NotifyEvent(LuaWaitForEvent e, Arg arg)
    {
        if (!m_awaiters.contains(e)) return;
        std::vector<AwaiterType>& awaiters = m_awaiters[e];

        for (int i = 0; i < awaiters.size(); i++)
        {
            auto [thread, coroutine, callback, predicate] = awaiters[i];

            if (predicate.has_value())
            {
                sol::protected_function_result result = predicate.value()(arg);
                if (!lua_utils::UnwrapResult<bool>(result, "Failed to run await predicate")) continue;
            }

            sol::protected_function_result result = coroutine();
            
            awaiters.erase(awaiters.begin() + i);
            i--;
            
            // coroutine has completed - no more yields
            if (coroutine.status() == sol::call_status::ok)
            {
                callback(result);
            }
            else
            {
                ContinueCoroutine(thread, coroutine, callback);
            }
        }

        if (awaiters.size() == 0)
        {
            m_awaiters.erase(e);
        }
    }
};
