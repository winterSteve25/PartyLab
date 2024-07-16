#pragma once

#include "raylib.h"
#include "UIElement.h"

class UIButton : public UIElement
{
public:
    UIButton(Vector2 pos, float font_size, const char* text, void(*onClicked)());
    void RenderOverlay() override;

protected:
    void OnEnterHover() override;
    void OnExitHover() override;
    void OnPressed() override;
    void OnClick() override;

private:
    float m_fontSize;
    float m_actualFontSize;
    Color m_color;
    Vector2 m_hoveredSize;
    const char* m_text;
    void(*m_onClicked)();
};
