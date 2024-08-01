#include "RatioProperty.h"

#include "ui/components/UIElement.h"

RatioProperty::RatioProperty():
    FloatProperty(-1, "ratio")
{
}

Vector2 RatioProperty::Apply(Vector2 size) const
{
    float width = size.x;
    float height = size.y;
    
    if (abs(m_val - m_defaultValue) < 0.01)
    {
        return {width, height};
    }

    if (abs(width) > 0 && abs(height) < 0.01)
    {
        height = width * (1 / m_val);
    }
    else if (abs(height) > 0 && abs(width) < 0.01)
    {
        width = height * m_val;
    }

    return {width, height};
}
