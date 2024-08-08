#pragma once

#include "layout.h"
#include "raylib.h"
#include "sol/sol.hpp"
#include "ui/Style.h"
#include "ui/properties/DirectionalProperty.h"
#include "ui/properties/RatioProperty.h"
#include "ui/properties/base/ColorProperty.h"
#include "ui/properties/base/LayFlagProperty.h"
#include "ui/properties/base/LengthProperty.h"

class UIElement
{
public:
    
    virtual ~UIElement();
    virtual void Update();
    virtual void Render(const lay_context* ctx);
    virtual void ApplyDefaultStyles();
    virtual void AddToLayout(lay_context* ctx, lay_id root);
    /**
     * This should not add any more elements to the layout, only modify the existing elements
     * @param ctx 
     */
    virtual void AdjustLayout(lay_context* ctx);
    virtual void ApplyStyles(const Style& style, bool doTransition);
    virtual sol::optional<UIElement*> Find(const std::string& id);
    virtual sol::table CreateLuaObject(lua_State* L);
    virtual void AddOffset(float x, float y);

    ColorProperty sBackgroundColor;
    LengthProperty sWidth;
    LengthProperty sHeight;
    DirectionalProperty sMarginLeft;
    DirectionalProperty sMarginRight;
    DirectionalProperty sMarginTop;
    DirectionalProperty sMarginBottom;
    LayFlagProperty sAlignSelf;
    LayFlagProperty sAlignItems;
    RatioProperty sRatio;

    lay_id id;
    bool markedDead;
    
    virtual void OnEnterHover();
    virtual void OnExitHover();
    virtual void OnClick();
    virtual void OnPressed();
    virtual void OnReleased();
    virtual void OnScrolled(const lay_context* ctx, float deltaX, float deltaY);

protected:
    UIElement(const sol::table& table);

    Style m_normalStyle;
    Style m_hoverStyle;
    Style m_pressStyle;
    
    float m_offsetX;
    float m_offsetY;
    
    std::optional<std::string> m_id;
    sol::optional<sol::table> m_customData;
    
    bool m_isHeldDown;
    bool m_isHovering;

    Vector2 GetPos(const lay_context* ctx) const;
    Vector2 GetSize(const lay_context* ctx) const;
private:
    void CheckEvents(const lay_context* ctx, const Vector2& pos, const Vector2& size);

    Color m_randItemColor;
    sol::optional<sol::protected_function> m_onDestroy;
    sol::optional<sol::protected_function> m_onClick;
};
