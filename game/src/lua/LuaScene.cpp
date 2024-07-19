#include "LuaScene.h"

#include "sol/sol.hpp"

LuaScene::LuaScene(
    const sol::optional<sol::function>& render,
    const sol::optional<sol::function>& render_overlay,
    const sol::optional<sol::function>& update,
    const sol::optional<sol::function>& load,
    const sol::optional<sol::function>& cleanup
): m_render(render),
   m_renderOverlay(render_overlay),
   m_update(update),
   m_load(load),
   m_cleanup(cleanup)
{
}

void LuaScene::Render()
{
    if (!m_render.has_value()) return;
    m_render.value().call<void>();
}

void LuaScene::RenderOverlay()
{
    if (!m_renderOverlay.has_value()) return;
    m_renderOverlay.value().call<void>();
}

void LuaScene::Update()
{
    if (!m_update.has_value()) return;
    m_update.value().call<void>();
}

void LuaScene::Load()
{
    if (!m_load.has_value()) return;
    m_load.value().call<void>();
}

void LuaScene::Cleanup()
{
    if (!m_cleanup.has_value()) return;
    m_cleanup.value().call<void>();
}
