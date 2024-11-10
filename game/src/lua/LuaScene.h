#pragma once

#include <format>

#include "lua_utils.h"
#include "sol/sol.hpp"
#include "ui/LuaUI.h"

/**
 * A scene created via lua
 */
class LuaScene
{
public:
    LuaScene(const sol::optional<sol::protected_function>& render, const sol::optional<sol::protected_function>& render_overlay,
        const sol::optional<sol::protected_function>& update, const sol::optional<sol::protected_function>& load,
        const sol::optional<sol::protected_function>& cleanup, const sol::optional<sol::protected_function>& ui,
        const sol::optional<sol::table>& eventHandlers);

    void Render();
    void RenderOverlay();
    void Update();
    void Load();
    void Cleanup();

    template <typename... Args>
    void ReceiveEvent(const std::string& event, const std::function<Args(sol::state_view*)>&... args)
    {
        if (!m_eventHandlers.has_value()) return;
        TraceLog(LOG_INFO, std::format("Scene Event '{}' was received", event).c_str());
        std::optional<sol::protected_function> handler = m_eventHandlers.value()[event];
        if (!handler.has_value()) return;
        sol::state_view stateView = m_eventHandlers.value().lua_state();
        sol::protected_function_result result = handler.value()(args(&stateView)...);
        lua_utils::UnwrapResult(result, "Failed to run event handler");
    }
    
private:
    static const std::string render_err;
    static const std::string overlay_err;
    static const std::string update_err;
    static const std::string load_err;
    static const std::string cleanup_err;
    
    sol::optional<sol::protected_function> m_render;
    sol::optional<sol::protected_function> m_renderOverlay;
    sol::optional<sol::protected_function> m_update;
    sol::optional<sol::protected_function> m_load;
    sol::optional<sol::protected_function> m_cleanup;
    sol::optional<sol::protected_function> m_ui;
    sol::optional<LuaUI*> m_uiBuilt;
    sol::optional<sol::table> m_eventHandlers;
};
