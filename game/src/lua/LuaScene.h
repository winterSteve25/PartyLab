#pragma once

#include "core/Scene.h"
#include "sol/forward.hpp"
#include "sol/sol.hpp"
#include "ui/LuaUI.h"

class LuaScene : public Scene
{
public:
    LuaScene(const sol::optional<sol::protected_function>& render, const sol::optional<sol::protected_function>& render_overlay,
        const sol::optional<sol::protected_function>& update, const sol::optional<sol::protected_function>& load,
        const sol::optional<sol::protected_function>& cleanup, const sol::optional<sol::protected_function>& ui);

    void Render() override;
    void RenderOverlay() override;
    void Update() override;
    void Load() override;
    void Cleanup() override;
private:
    sol::optional<sol::protected_function> m_render;
    sol::optional<sol::protected_function> m_renderOverlay;
    sol::optional<sol::protected_function> m_update;
    sol::optional<sol::protected_function> m_load;
    sol::optional<sol::protected_function> m_cleanup;
    sol::optional<sol::protected_function> m_ui;
    sol::optional<LuaUI*> m_uiBuilt;
};
