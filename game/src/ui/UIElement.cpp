#include "UIElement.h"

#include "ui_helper.h"

UIElement::UIElement(Vector2 pos, Vector2 size):
    m_pos(pos),
    m_size(size),
    m_isHeldDown(false),
    m_wasHovered(false)
{
}

void UIElement::Render()
{
    bool within = ui_helper::Within(GetMousePosition(), m_pos, m_size);
    
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

        if (m_wasHovered) return;
        m_wasHovered = true;
        OnHover();
    }
    else
    {
        if (!m_wasHovered) return;
        m_wasHovered = false;
    }
}

void UIElement::OnHover()
{
    TraceLog(LOG_INFO, "hovered");
}

void UIElement::OnClick()
{
    TraceLog(LOG_INFO, "Clicked");
}

void UIElement::OnPressed()
{
    TraceLog(LOG_INFO, "Pressed");
}
