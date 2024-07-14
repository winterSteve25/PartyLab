#pragma once
#include "raylib.h"

class UIElement
{
public:
    UIElement(Vector2 pos, Vector2 size);
    virtual ~UIElement() = default;
    virtual void Render();
protected:
    virtual void OnHover();
    virtual void OnClick();
    virtual void OnPressed();
    
    Vector2 m_pos;
    Vector2 m_size;
    bool m_isHeldDown;
private:
    bool m_wasHovered;
};
