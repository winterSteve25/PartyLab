#include "UIRendered.h"

#include "lua/lua_utils.h"

UIRendered::UIRendered(const sol::table& table): UIElement(table)
{
    m_renderer = table.get<sol::optional<sol::protected_function>>("render");
}

void UIRendered::Render(const lay_context* pos)
{
    UIElement::Render(pos);
    if (!m_renderer.has_value()) return;
    lua_utils::UnwrapResult(m_renderer.value()(), "Failed to call render callback");
}
