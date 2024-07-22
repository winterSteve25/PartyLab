#include "UIText.h"

#include "core/Core.h"
#include "ui/ui_helper.h"

UIText::UIText(const sol::table& table, const std::string& text) :
    UIElement(table),
    m_text(text),
    m_sColor(ColorProperty(this)),
    m_sFontSize(FontSizeProperty(this, &this->m_text))
{
}

void UIText::RenderOverlay()
{
    UIElement::RenderOverlay();
    Vector2 pos = m_sPos;
    // pos.y -= pos.y * 0.5f;
    ui_helper::DrawText(m_text.c_str(), m_sFontSize.Get(), pos, m_sColor.Get());
}

void UIText::ApplyStyles(const Style& style, bool doTransition)
{
    UIElement::ApplyStyles(style, doTransition);
    m_sColor.Set(style, doTransition);
    m_sFontSize.Set(style, doTransition);
}
