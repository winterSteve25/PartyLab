#pragma once
#include <string>

#include "UIElement.h"
#include "ui/properties/FontSizeProperty.h"
#include "ui/properties/base/ColorProperty.h"

class UIText : public UIElement
{
public:
    UIText(const sol::table& table, const std::string& text);
    UIText(const sol::table& table);
    void Render(const lay_context* pos) override;
    sol::table CreateLuaObject(lua_State* L) override;

protected:
    void ApplyStyles(const Style& style, bool doTransition) override;
    std::string m_text;
    ColorProperty m_sColor;
    FontSizeProperty m_sFontSize;
};
