#pragma once
#include "raylib.h"
#include "core/Renderable.h"

class UIElement : public Renderable
{
public:
    UIElement(Vector2 pos, Vector2 size);
    virtual ~UIElement() = default;
    void Update() override;
    void Render() override;
protected:
    virtual void OnEnterHover();
    virtual void OnExitHover();
    virtual void OnClick();
    virtual void OnPressed();
    
    Vector2 m_pos;
    Vector2 m_size;
    bool m_isHeldDown;
    bool m_isHovering;
private:
};
