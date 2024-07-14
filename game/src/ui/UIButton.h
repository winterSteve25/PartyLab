#pragma once

#include "raylib.h"
#include "UIElement.h"

class UIButton : public UIElement
{
public:
    void Render();
    UIButton(Vector2 pos, float font_size, const char* text, void(*onClicked)());

protected:
    void OnHover() override;
    void OnClick() override;
    void OnPressed() override;

private:
    float m_fontSize;
    Vector2 m_hoveredSize;
    const char* m_text;
    void(*m_onClicked)();
};
