#include "LuaScene.h"

#include "LuaMod.h"
#include "sol/sol.hpp"

const std::string LuaScene::render_err = "Failed to render";
const std::string LuaScene::overlay_err = "Failed to render overlay";
const std::string LuaScene::load_err = "Failed to load a custom lua scene";
const std::string LuaScene::cleanup_err = "Failed to clean up";
const std::string LuaScene::update_err = "Failed to update";

LuaScene::LuaScene(
    const sol::optional<sol::protected_function>& render,
    const sol::optional<sol::protected_function>& render_overlay,
    const sol::optional<sol::protected_function>& update,
    const sol::optional<sol::protected_function>& load,
    const sol::optional<sol::protected_function>& cleanup,
    const sol::optional<sol::protected_function>& ui,
    const sol::optional<sol::table>& eventHandlers
): m_render(render),
   m_renderOverlay(render_overlay),
   m_update(update),
   m_load(load),
   m_cleanup(cleanup),
   m_ui(ui),
   m_uiBuilt(sol::optional<LuaUI*>(sol::nullopt)),
   m_eventHandlers(eventHandlers)
{
}

void LuaScene::Render()
{
    if (!m_render.has_value()) return;
    lua_utils::UnwrapResult(m_render.value()(), render_err);
}

void LuaScene::RenderOverlay()
{
    if (m_uiBuilt.has_value())
    {
        m_uiBuilt.value()->Render();
    }

    if (m_renderOverlay.has_value())
    {
        lua_utils::UnwrapResult(m_renderOverlay.value()(), overlay_err);
    }
}

void LuaScene::Update()
{
    if (m_uiBuilt.has_value())
    {
        m_uiBuilt.value()->Update();
    }

    if (!m_update.has_value()) return;
    lua_utils::UnwrapResult(m_update.value()(), update_err);
}

void LuaScene::Load()
{
    if (m_load.has_value())
    {
        lua_utils::UnwrapResult(m_load.value()(), load_err);
    }

    if (!m_ui.has_value()) return;
    m_uiBuilt = new LuaUI(m_ui.value());
}

void LuaScene::Cleanup()
{
    if (m_uiBuilt.has_value())
    {
        delete m_uiBuilt.value();
        m_uiBuilt = sol::optional<LuaUI*>(sol::nullopt);
    }

    if (!m_cleanup.has_value()) return;
    lua_utils::UnwrapResult(m_cleanup.value()(), cleanup_err);
}
