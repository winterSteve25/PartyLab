#include "UIElement.h"

#include "raymath.h"
#include "lua/LuaConstants.h"
#include "ui/ui_helper.h"

UIElement::UIElement(const sol::table& table):
    m_sPos(Vector2Zero()),
    m_sSize(Vector2Zero()),
    m_isHeldDown(false),
    m_isHovering(false),
    m_sBackgroundColor(BackgroundColorProperty(this)),
    m_randItemColor(Color(GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255))
{
    sol::optional<sol::table> styles = table[UI_PROP_STYLE];
    m_normalStyle = Style(styles);
    m_hoverStyle = Style(Style::GetOptionalField<sol::table>(styles, UI_PROP_STYLE_HOVER));
    m_pressedStyle = Style(Style::GetOptionalField<sol::table>(styles, UI_PROP_STYLE_PRESS));
}

void UIElement::Init()
{
    Style& style = m_normalStyle;

    if (m_isHovering)
    {
        style = m_hoverStyle.InheritFrom(style);
    }

    if (m_isHeldDown)
    {
        style = m_pressedStyle.InheritFrom(style);
    }

    ApplyStyles(style, false);
}

void UIElement::Render()
{
}

void UIElement::RenderOverlay()
{
    if (m_sBackgroundColor.Get().a != 0)
    {
        DrawRectangle(m_sPos.x, m_sPos.y, m_sSize.x, m_sSize.y, m_sBackgroundColor.Get());
    }

    if (!IsKeyDown(KEY_LEFT_CONTROL) || !IsKeyDown(KEY_E)) return;
    lay_vec4 margin = lay_get_margins(layCtx, layId);

    if (margin[0] != 0 || margin[1] != 0 || margin[2] != 0 || margin[3] != 0)
    {
        DrawRectangleLines(m_sPos.x - margin[0], m_sPos.y - margin[1], m_sPos.x + m_sSize.x + margin[2],
                      m_sPos.y + m_sSize.y + margin[3], m_randItemColor);
    }

    DrawRectangleLines(m_sPos.x, m_sPos.y, m_sSize.x, m_sSize.y, m_randItemColor);
}

void UIElement::Update()
{
    auto rect = lay_get_rect(layCtx, layId);
    m_sPos = Vector2(rect[0], rect[1]);
    m_sSize = Vector2(rect[2], rect[3]);

    bool within = ui_helper::Within(GetMousePosition(), m_sPos, m_sSize);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        if (m_isHeldDown && within)
        {
            OnClick();
        }

        m_isHeldDown = false;
    }

    if (within)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            m_isHeldDown = true;
            OnPressed();
        }

        if (m_isHovering) return;
        m_isHovering = true;
        OnEnterHover();
    }
    else
    {
        if (!m_isHovering) return;
        m_isHovering = false;
        OnExitHover();
    }
}

void UIElement::OnEnterHover()
{
    ApplyStyles(m_hoverStyle.InheritFrom(m_normalStyle), true);
}

void UIElement::OnExitHover()
{
    ApplyStyles(m_normalStyle, true);
}

void UIElement::OnPressed()
{
    ApplyStyles(m_pressedStyle.InheritFrom(m_hoverStyle.InheritFrom(m_normalStyle)), true);
}

void UIElement::OnReleased()
{
    if (m_isHovering)
    {
        ApplyStyles(m_hoverStyle.InheritFrom(m_normalStyle), true);
    }
    else
    {
        ApplyStyles(m_normalStyle, true);
    }
}

void UIElement::ApplyStyles(const Style& style, bool doTransition)
{
    m_sBackgroundColor.Set(style, doTransition);

    auto parent = lay_get_rect(layCtx, layParent);
    int parentWidth = parent[2];
    int parentHeight = parent[3];
    int width = style.width.val.Eval(parentWidth);
    int height = style.height.val.Eval(parentHeight);

    lay_set_size_xy(layCtx, layId, width, height);
    lay_set_contain(layCtx, layId, style.alignItems.val);
    lay_set_behave(layCtx, layId, style.alignSelf.val);
    lay_set_margins_ltrb(
        layCtx,
        layId,
        style.marginLeft.val.Eval(parentWidth),
        style.marginTop.val.Eval(parentHeight),
        style.marginRight.val.Eval(parentWidth),
        style.marginBottom.val.Eval(parentHeight)
    );
}

void UIElement::OnClick()
{
}
