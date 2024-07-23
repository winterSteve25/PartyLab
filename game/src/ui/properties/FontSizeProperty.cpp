#include "FontSizeProperty.h"
#include "core/Core.h"
#include "ui/ui_helper.h"
#include "ui/components/UIElement.h"

FontSizeProperty::FontSizeProperty(UIElement* parent, const std::string* text):
    FloatProperty(64, "fontSize"),
    m_parentText(text),
    m_parent(parent)
{
}

void FontSizeProperty::OnSet()
{
    Vector2 size = ui_helper::MeasureText(this->m_parentText->c_str(), m_val);
    this->m_parent->sWidth.Override(size.x);
    this->m_parent->sHeight.Override(size.y);
}

float FontSizeProperty::GetNewValue(const Style& style)
{
    float newFontSize = FloatProperty::GetNewValue(style);
    
    float hRatio = static_cast<float>(GetScreenWidth()) / 1920.0f;
    float vRatio = static_cast<float>(GetScreenHeight()) / 1080.0f;
    
    return newFontSize * (hRatio + vRatio) * 0.5f;
}
