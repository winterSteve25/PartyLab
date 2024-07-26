#include "Size.h"

#include "raylib.h"

Size::Size(): m_type(ABSOLUTE), m_measure(0)
{
}

Size::Size(const sol::object& size)
{
    if (size.is<int>())
    {
        int f = size.as<int>();
        m_type = ABSOLUTE;
        m_measure = f;
    }
    else if (size.is<float>())
    {
        float f = size.as<float>();
        m_type = VIEW;
        m_measure = f;
    }
    else
    {
        TraceLog(LOG_ERROR, "Size should be either float or int to represent a percentage or absolute value");
    }
}

Size::Size(SizeType type, float measure): m_type(type), m_measure(measure)
{
}

int Size::Eval(int max) const
{
    if (m_type == ABSOLUTE)
    {
        return m_measure;
    }

    int i = m_measure * max;
    return i;
}
