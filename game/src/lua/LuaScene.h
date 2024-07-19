#pragma once

#include "core/Scene.h"
#include "sol/forward.hpp"
#include "sol/sol.hpp"

class LuaScene : public Scene
{
public:
    LuaScene(const sol::optional<sol::function>& render, const sol::optional<sol::function>& render_overlay,
        const sol::optional<sol::function>& update, const sol::optional<sol::function>& load,
        const sol::optional<sol::function>& cleanup);

    void Render() override;
    void RenderOverlay() override;
    void Update() override;
    void Load() override;
    void Cleanup() override;
private:
    sol::optional<sol::function> m_render;
    sol::optional<sol::function> m_renderOverlay;
    sol::optional<sol::function> m_update;
    sol::optional<sol::function> m_load;
    sol::optional<sol::function> m_cleanup;
};
