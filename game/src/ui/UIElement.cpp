#include "UIElement.h"

#include "ui/ui_helper.h"

UIElement::UIElement(Vector2 pos, Vector2 size):
    m_pos(pos),
    m_size(size),
    m_isHeldDown(false),
    m_isHovering(false)
{
}

void UIElement::Render()
{
}

void UIElement::Update()
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
}

void UIElement::OnExitHover()
{
    TraceLog(LOG_INFO, "Exit Hover");
}

void UIElement::OnClick()
{
}

void UIElement::OnPressed()
{
}
