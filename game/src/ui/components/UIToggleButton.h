#pragma once
#include "UIText.h"

class UIToggleButton : public UIText
{
public:
    UIToggleButton(const sol::table& table);
protected:
    void OnClick() override;
private:
    sol::optional<sol::protected_function> m_onToggled;
    bool m_value;
    std::string m_idleText;
    std::string m_toggledText;
};
