#include "LuaAsyncManager.h"

#include "raylib.h"
#include "WaitFor.h"
#include "lua/lua_utils.h"

void LuaAsyncManager::ContinueCoroutine(const sol::thread& thread, sol::coroutine& coroutine, const std::function<void(const sol::protected_function_result&)>& callback)
{
    sol::protected_function_result result = coroutine();

    // coroutine finished
    if (coroutine.status() == sol::call_status::ok)
    {
        callback(result);
        return;
    }
    
    using pair = std::tuple<sol::object, sol::optional<sol::protected_function>>;
    auto [first, second] = lua_utils::UnwrapResult<pair>(result, "Failed to continue coroutine");
    
    if (!first.is<WaitFor>())
    {
        TraceLog(LOG_WARNING, "Tried to use a non async coroutine as an async coroutine");
        return;
    }

    WaitFor waitfor = first.as<WaitFor>();
    if (!m_awaiters.contains(waitfor.e))
    {
        m_awaiters[waitfor.e] = std::vector<AwaiterType>();
    }

    m_awaiters[waitfor.e].emplace_back(thread, coroutine, callback, second);
}

void LuaAsyncManager::CallAsync(const sol::coroutine& coroutine, const std::function<void(const sol::protected_function_result&)>& callback)
{
    sol::thread thread = sol::thread::create(coroutine.lua_state());
    sol::coroutine newCoroutine = sol::coroutine(thread.state(), coroutine);
    ContinueCoroutine(thread, newCoroutine, callback);
}
