#include "LuaScene.h"

#include "LuaMod.h"
#include "sol/sol.hpp"

LuaScene::LuaScene(
    const sol::optional<sol::protected_function>& render,
    const sol::optional<sol::protected_function>& render_overlay,
    const sol::optional<sol::protected_function>& update,
    const sol::optional<sol::protected_function>& load,
    const sol::optional<sol::protected_function>& cleanup,
    const sol::optional<sol::table>& ui
): m_render(render),
   m_renderOverlay(render_overlay),
   m_update(update),
   m_load(load),
   m_cleanup(cleanup),
   m_ui(ui),
   m_uiBuilt(sol::optional<LuaUI*>(sol::nullopt))
{
}

void LuaScene::Render()
{
    if (m_uiBuilt.has_value())
    {
        m_uiBuilt.value()->Render();
    }

    if (!m_render.has_value()) return;
    lua_utils::UnwrapResult(m_render.value()(), "Failed to render");
}

void LuaScene::RenderOverlay()
{
    if (m_uiBuilt.has_value())
    {
        m_uiBuilt.value()->RenderOverlay();
    }

    if (!m_renderOverlay.has_value()) return;
    lua_utils::UnwrapResult(m_renderOverlay.value()(), "Failed to render overlay");
}

void LuaScene::Update()
{
    if (m_uiBuilt.has_value())
    {
        m_uiBuilt.value()->Update();
    }

    if (!m_update.has_value()) return;
    lua_utils::UnwrapResult(m_update.value()(), "Failed to update");
}

void LuaScene::Load()
{
    if (m_ui.has_value())
    {
        LuaUI* ui = new LuaUI(m_ui.value());
        m_uiBuilt.emplace(ui);
    }

    if (!m_load.has_value()) return;
    lua_utils::UnwrapResult(m_load.value()(), "Failed to load");
}

void LuaScene::Cleanup()
{
    if (m_uiBuilt.has_value())
    {
        delete m_uiBuilt.value();
    }
    
    if (!m_cleanup.has_value()) return;
    lua_utils::UnwrapResult(m_cleanup.value()(), "Failed to clean up");
}
