#pragma once

#include "layout.h"
#include "raylib.h"
#include "sol/sol.hpp"
#include "ui/Style.h"
#include "ui/properties/DirectionalProperty.h"
#include "ui/properties/base/ColorProperty.h"
#include "ui/properties/base/LayFlagProperty.h"
#include "ui/properties/base/SizeProperty.h"

class UIElement
{
public:
    virtual ~UIElement() = default;
    virtual void Render(const lay_context* ctx);
    virtual void ApplyDefaultStyles();
    virtual void AddToLayout(lay_context* ctx, lay_id root);
    virtual void ApplyStyles(const Style& style, bool doTransition);

    ColorProperty sBackgroundColor;
    SizeProperty sWidth;
    SizeProperty sHeight;
    DirectionalProperty sMarginLeft;
    DirectionalProperty sMarginRight;
    DirectionalProperty sMarginTop;
    DirectionalProperty sMarginBottom;
    LayFlagProperty sAlignSelf;
    LayFlagProperty sAlignItems;

    lay_id id;
protected:
    UIElement(const sol::table& table);

    Style m_normalStyle;
    Style m_hoverStyle;
    Style m_pressStyle;
    
    virtual void OnEnterHover();
    virtual void OnExitHover();
    virtual void OnClick();
    virtual void OnPressed();
    virtual void OnReleased();

    Vector2 GetPos(const lay_context* ctx) const;
    Vector2 GetSize(const lay_context* ctx) const;
private:
    void CheckEvents(const Vector2& pos);
    
    bool m_isHeldDown;
    bool m_isHovering;
    Color m_randItemColor;
};
