#include "UIElement.h"

#include <format>

#include "layout.h"
#include "ui/ui_helper.h"
#include "ui/properties/properties.h"

#define SCALER_CAST(x) static_cast<lay_scalar>(x)

UIElement::UIElement(const sol::table& table):
    sBackgroundColor(properties::BackgroundColorProp({0, 0, 0, 0})),
    sWidth(properties::WidthProp()),
    sHeight(properties::HeightProp()),
    sMarginLeft(properties::MarginLeft()),
    sMarginRight(properties::MarginRight()),
    sMarginTop(properties::MarginTop()),
    sMarginBottom(properties::MarginBottom()),
    sAlignSelf(LayFlagProperty(LAY_TOP, "alignSelf")),
    sAlignItems(LayFlagProperty(LAY_ROW, "alignItems")),
    m_normalStyle(Style(table.lua_state(), table["style"])),
    m_hoverStyle(Style(table.lua_state(), m_normalStyle.Get<sol::table>("hovered"))),
    m_pressStyle(Style(table.lua_state(), m_normalStyle.Get<sol::table>("pressed"))),
    m_offsetX(0),
    m_offsetY(0),
    m_id(table.get<std::optional<std::string>>("id")),
    m_isHeldDown(false),
    m_isHovering(false),
    m_randItemColor(Color(GetRandomValue(0, 255), GetRandomValue(0, 255),
                          GetRandomValue(0, 255), 255))
{
}

void UIElement::ApplyDefaultStyles()
{
    if (m_isHovering)
    {
        if (m_isHeldDown)
        {
            ApplyStyles(m_pressStyle.InheritFrom(m_hoverStyle.InheritFrom(m_normalStyle)), false);
            return;
        }

        ApplyStyles(m_hoverStyle.InheritFrom(m_normalStyle), false);
        return;
    }

    ApplyStyles(m_normalStyle, false);
}

void UIElement::AddToLayout(lay_context* ctx, lay_id root)
{
    id = lay_item(ctx);

    lay_set_size_xy(ctx, id, SCALER_CAST(sWidth.Get()), SCALER_CAST(sHeight.Get()));
    lay_set_behave(ctx, id, sAlignSelf.Get());
    lay_set_contain(ctx, id, sAlignItems.Get());
    lay_set_margins_ltrb(ctx, id, SCALER_CAST(sMarginLeft.Get()), SCALER_CAST(sMarginTop.Get()),
                         SCALER_CAST(sMarginRight.Get()), SCALER_CAST(sMarginBottom.Get()));

    lay_insert(ctx, root, id);
}

void UIElement::AdjustLayout(lay_context* ctx)
{
    Vector2 size = sRatio.Apply(GetSize(ctx));
    lay_set_size_xy(ctx, id, SCALER_CAST(size.x), SCALER_CAST(size.y));
}

void UIElement::Update()
{
}

void UIElement::Render(const lay_context* ctx)
{
    Vector2 pos = GetPos(ctx);
    Vector2 size = GetSize(ctx);

    CheckEvents(ctx, pos, size);

    const float width = size.x;
    const float height = size.y;

    if (sBackgroundColor.Get().a != 0)
    {
        DrawRectangle(pos.x, pos.y, width, height, sBackgroundColor.Get());
    }

    if (!IsKeyDown(KEY_LEFT_CONTROL) || !IsKeyDown(KEY_E)) return;
    DrawRectangleLinesEx({pos.x, pos.y, width, height}, 2, m_randItemColor);
}

void UIElement::CheckEvents(const lay_context* ctx, const Vector2& pos, const Vector2& size)
{
    bool within = ui_helper::Within(GetMousePosition(), pos, size);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        if (m_isHeldDown && within)
        {
            OnClick();
            OnReleased();

            if (m_isHovering)
            {
                ApplyStyles(m_hoverStyle.InheritFrom(m_normalStyle), true);
            }
            else
            {
                ApplyStyles(m_normalStyle, true);
            }
        }

        m_isHeldDown = false;
    }

    if (within)
    {
        auto wheel = GetMouseWheelMoveV();
        if (fabs(wheel.x) > 0 || fabs(wheel.y) > 0)
        {
            OnScrolled(ctx, wheel.x, wheel.y);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            m_isHeldDown = true;
            if (m_isHovering)
            {
                ApplyStyles(m_pressStyle.InheritFrom(m_hoverStyle.InheritFrom(m_normalStyle)), true);
            }
            OnPressed();
        }

        if (m_isHovering) return;
        m_isHovering = true;

        if (m_isHeldDown)
        {
            ApplyStyles(m_pressStyle.InheritFrom(m_hoverStyle.InheritFrom(m_normalStyle)), true);
        }
        else
        {
            ApplyStyles(m_hoverStyle.InheritFrom(m_normalStyle), true);
        }

        OnEnterHover();
    }
    else
    {
        if (!m_isHovering) return;
        m_isHovering = false;
        ApplyStyles(m_normalStyle, true);
        OnExitHover();
    }
}

void UIElement::OnEnterHover()
{
}

void UIElement::OnExitHover()
{
}

void UIElement::OnPressed()
{
}

void UIElement::OnReleased()
{
}

void UIElement::OnClick()
{
}

void UIElement::OnScrolled(const lay_context* ctx, float deltaX, float deltaY)
{
}

void UIElement::ApplyStyles(const Style& style, bool doTransition)
{
    sBackgroundColor.Set(style, doTransition);
    sWidth.Set(style, doTransition);
    sHeight.Set(style, doTransition);
    sMarginTop.Set(style, doTransition);
    sMarginBottom.Set(style, doTransition);
    sMarginRight.Set(style, doTransition);
    sMarginLeft.Set(style, doTransition);
    sAlignSelf.Set(style, doTransition);
    sAlignItems.Set(style, doTransition);
    sRatio.Set(style, doTransition);
}

sol::optional<UIElement*> UIElement::Find(const std::string& id)
{
    if (m_id.has_value() && m_id.value() == id)
    {
        return this;
    }

    return sol::optional<UIElement*>(sol::nullopt);
}

sol::table UIElement::CreateLuaObject(lua_State* L)
{
    return sol::state::create_table(L);
}

void UIElement::AddOffset(float x, float y)
{
    m_offsetX += x;
    m_offsetY += y;
}

Vector2 UIElement::GetPos(const lay_context* ctx) const
{
    auto rect = lay_get_rect(ctx, id);
    float x = static_cast<float>(rect[0]) + m_offsetX;
    float y = static_cast<float>(rect[1]) + m_offsetY;
    return {x, y};
}

Vector2 UIElement::GetSize(const lay_context* ctx) const
{
    auto rect = lay_get_rect(ctx, id);
    float x = static_cast<float>(rect[2]);
    float y = static_cast<float>(rect[3]);
    return { x, y };
}
