#include "UIButton.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"

UIButton::UIButton(const sol::table& table): UIText(table)
{
    m_onClicked = table.get<sol::optional<sol::protected_function>>("onClick");
}

void UIButton::OnClick()
{
    UIElement::OnClick();
    if (!m_onClicked.has_value()) return;
    lua_utils::UnwrapResult(m_onClicked.value()(), "Failed to call click callback");
}