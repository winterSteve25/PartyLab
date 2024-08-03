#include "UIRendered.h"

#include "lua/lua_utils.h"

UIRendered::UIRendered(const sol::table& table): UIElement(table)
{
    m_renderer = table.get<sol::optional<sol::protected_function>>("render");
}

void UIRendered::Render(const lay_context* ctx)
{
    UIElement::Render(ctx);
    if (!m_renderer.has_value()) return;
    lua_utils::UnwrapResult(m_renderer.value()(GetPos(ctx), GetSize(ctx), m_customData), "Failed to call render callback");
}
