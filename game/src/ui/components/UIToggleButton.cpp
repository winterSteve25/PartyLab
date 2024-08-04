#include "UIToggleButton.h"

#include "lua/lua_utils.h"

UIToggleButton::UIToggleButton(const sol::table& table):
    UIText(table),
    m_value(false),
    m_idleText(table.get<sol::optional<std::string>>("idleText").value_or("Idle")),
    m_toggledText(table.get<sol::optional<std::string>>("toggledText").value_or("Toggled")),
    m_onToggled(table.get<sol::optional<sol::protected_function>>("onToggle"))
{
    m_text = m_idleText;
}

void UIToggleButton::OnClick()
{
    m_value = !m_value;
    m_text = m_value ? m_toggledText : m_idleText;
    ApplyDefaultStyles();
    if (!m_onToggled.has_value()) return;
    lua_utils::UnwrapResult(m_onToggled.value()(m_value), "Failed to call on toggle callback");
}
