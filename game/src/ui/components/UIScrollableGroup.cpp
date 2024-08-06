#include "UIScrollableGroup.h"

#include "raymath.h"

UIScrollableGroup::UIScrollableGroup(const sol::table& table):
    UIGroup(table),
    m_targetScrollOffsetX(0),
    m_targetScrollOffsetY(0),
    m_minScrollX(0),
    m_minScrollY(0),
    m_scrollBufferLimit(0),
    m_overflowX(false),
    m_overflowY(false),
    m_targetPercentageX(-1),
    m_targetPercentageY(-1)
{
}

void UIScrollableGroup::Update()
{
    m_scrollBufferLimit = GetScreenHeight() * 0.05f;
    UIGroup::Update();
}

void UIScrollableGroup::Render(const lay_context* ctx)
{
    const lay_id lastChild = lay_last_child(ctx, id);
    if (lastChild != LAY_INVALID_ID)
    {
        lay_vec4 lastChildRect = lay_get_rect(ctx, lastChild);
        lay_vec4 lastChildMargin = lay_get_margins(ctx, lastChild);
        lay_vec4 thisRect = lay_get_rect(ctx, id);

        float lastChildRight = lastChildRect[0] + lastChildRect[2] + lastChildMargin[2];
        float lastChildBottom = lastChildRect[1] + lastChildRect[3] + lastChildMargin[3];

        m_overflowX = lastChildRight > thisRect[0] + thisRect[2];
        m_overflowY = lastChildBottom > thisRect[1] + thisRect[3];

        m_minScrollX = -(lastChildRight - thisRect[0] - thisRect[2]);
        m_minScrollY = -(lastChildBottom - thisRect[1] - thisRect[3]);

        m_scrollOffsetX += (m_targetScrollOffsetX - m_scrollOffsetX) * 0.2f;
        m_scrollOffsetY += (m_targetScrollOffsetY - m_scrollOffsetY) * 0.2f;

        if (m_overflowX)
        {
            if (abs(m_scrollOffsetX) < 0.001)
            {
                m_scrollOffsetX = 0;
            }

            if (abs(m_scrollOffsetX - m_targetScrollOffsetX) < 5)
            {
                if (m_scrollOffsetX > 0)
                {
                    m_targetScrollOffsetX = 0;
                }
                else if (m_scrollOffsetX < m_minScrollX)
                {
                    m_targetScrollOffsetX = m_minScrollX;
                }
            }
        }

        if (abs(m_scrollOffsetY) < 0.001)
        {
            m_scrollOffsetY = 0;
        }

        if (abs(m_scrollOffsetY - m_targetScrollOffsetY) < 5)
        {
            if (m_scrollOffsetY > 0)
            {
                m_targetScrollOffsetY = 0;
            }
            else if (m_scrollOffsetY < m_minScrollY && m_overflowY)
            {
                m_targetScrollOffsetY = m_minScrollY;
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && m_isHovering)
        {
            Vector2 delta = GetMouseDelta();
            if (m_overflowX)
            {
                m_targetScrollOffsetX += delta.x;
                m_targetScrollOffsetX = Clamp(m_targetScrollOffsetX, m_minScrollX - m_scrollBufferLimit,
                                              m_scrollBufferLimit);
            }

            if ((delta.y < 0 && m_overflowY) || delta.y > 0)
            {
                m_targetScrollOffsetY += delta.y;
                m_targetScrollOffsetY = Clamp(m_targetScrollOffsetY, m_minScrollY - m_scrollBufferLimit,
                                              m_scrollBufferLimit);
            }
        }
    }

    if (abs(m_targetPercentageX + 1) > 0.01)
    {
        if (m_overflowX)
        {
            m_targetScrollOffsetX = m_targetPercentageX * m_minScrollX;
        }
        m_targetPercentageX = -1;
    }

    if (abs(m_targetPercentageY + 1) > 0.01)
    {
        if (m_overflowY)
        {
            m_targetScrollOffsetY = m_targetPercentageY * m_minScrollY;
        }
        m_targetPercentageY = -1;
    }

    UIGroup::Render(ctx);
}

sol::table UIScrollableGroup::CreateLuaObject(lua_State* L)
{
    sol::table t = UIGroup::CreateLuaObject(L);

    t["scrollToX"] = [this](const float& percentage)
    {
        m_targetPercentageX = percentage;
    };

    t["scrollToY"] = [this](const float& percentage)
    {
        m_targetPercentageY = percentage;
    };

    return t;
}

void UIScrollableGroup::OnScrolled(const lay_context* ctx, float deltaX, float deltaY)
{
    constexpr float multiplier = 20.0f;
    const lay_id lastChild = lay_last_child(ctx, id);

    // no children
    if (lastChild == LAY_INVALID_ID) return;

    if (deltaX < 0
        && m_targetScrollOffsetX > m_minScrollX - m_scrollBufferLimit
        && m_overflowX)
    {
        m_targetScrollOffsetX += deltaX * multiplier;
    }

    if (deltaY < 0
        && m_targetScrollOffsetY > m_minScrollY - m_scrollBufferLimit
        && m_overflowY)
    {
        m_targetScrollOffsetY += deltaY * multiplier;
    }

    if (deltaX > 0
        && m_targetScrollOffsetX < m_scrollBufferLimit
        && m_overflowX)
    {
        m_targetScrollOffsetX += deltaX * multiplier;
    }

    if (deltaY > 0
        && m_targetScrollOffsetY < m_scrollBufferLimit
        && m_overflowY)
    {
        m_targetScrollOffsetY += deltaY * multiplier;
    }
}
