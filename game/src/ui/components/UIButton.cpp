#include "UIButton.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"

UIButton::UIButton(const sol::table& table):
    UIText(table, std::string(""))
{
    if (sol::optional<std::string> text = table[UI_BUTTON_PROP_TEXT]; text.has_value())
    {
        m_text = text.value();
    }
    
    m_onClicked = table[UI_BUTTON_PROP_ONCLICK];
}

void UIButton::OnClick()
{
    UIElement::OnClick();
    if (!m_onClicked.has_value()) return;
    lua_utils::UnwrapResult(m_onClicked.value()(), "Error when clicked");
}