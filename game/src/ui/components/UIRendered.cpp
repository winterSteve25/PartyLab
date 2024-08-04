#include "UIRendered.h"

#include "lua/lua_utils.h"

const std::string UIRendered::fail_callback_txt = "Failed to call render callback";
UIRendered::UIRendered(const sol::table& table): UIElement(table)
{
    m_renderer = table.get<sol::protected_function>("render");
}

void UIRendered::Render(const lay_context* ctx)
{
    UIElement::Render(ctx);
    lua_utils::UnwrapResult(m_renderer(GetPos(ctx), GetSize(ctx), m_customData), fail_callback_txt);
}
