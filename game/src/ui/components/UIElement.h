#pragma once

#include "layout.h"
#include "raylib.h"
#include "core/Renderable.h"
#include "sol/sol.hpp"
#include "ui/properties/BackgroundColorProperty.h"
#include "ui/styles/Style.h"

class UIElement : public Renderable
{
public:
    ~UIElement() override = default;
    void Update() override;
    void Render() override;
    void RenderOverlay() override;
    virtual void Init();

    lay_context* layCtx;
    lay_id layId;
    lay_id layParent;
protected:
    UIElement(const sol::table& table);

    Vector2 m_sPos;
    Vector2 m_sSize;
    
    Style m_normalStyle;
    Style m_hoverStyle;
    Style m_pressedStyle;

    virtual void OnEnterHover();
    virtual void OnExitHover();
    virtual void OnClick();
    virtual void OnPressed();
    virtual void OnReleased();
    virtual void ApplyStyles(const Style& style, bool doTransition);
private:
    bool m_isHeldDown;
    bool m_isHovering;
    BackgroundColorProperty m_sBackgroundColor;
    Color m_randItemColor;
};
