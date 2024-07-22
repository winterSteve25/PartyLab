#pragma once
#include <string>

#include "UIElement.h"
#include "ui/properties/FontSizeProperty.h"

class UIText : public UIElement
{
public:
    UIText(const sol::table& table, const std::string& text);
    ~UIText() override = default;

    void RenderOverlay() override;

protected:
    void ApplyStyles(const Style& style, bool doTransition) override;
    std::string m_text;
    ColorProperty m_sColor;
    FontSizeProperty m_sFontSize;
};
