#pragma once

#include "UIText.h"

class UIButton : public UIText
{
public:
    UIButton(const sol::table& table);
protected:
    void OnClick() override;
private:
    sol::optional<sol::protected_function> m_onClicked;
};
