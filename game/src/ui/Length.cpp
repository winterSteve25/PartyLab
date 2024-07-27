#include "Length.h"

#include <format>

#include "raylib.h"

Length::Length(): m_type(ABSOLUTE_SIZE), m_measure(0)
{
}

Length::Length(const sol::object& size)
{
    if (size.is<int>() && size.as<int>() != 0)
    {
        int f = size.as<int>();
        m_type = ABSOLUTE_SIZE;
        m_measure = f;
    }
    else if (size.is<float>())
    {
        float f = size.as<float>();
        m_type = PERCENT_SIZE;
        m_measure = f;
    }
    else
    {
        TraceLog(LOG_ERROR, "Size should be either float or int to represent a percentage or absolute value");
    }
}

Length::Length(int type, float measure): m_type(type), m_measure(measure)
{
}

int Length::Eval(int max) const
{
    if (m_type == ABSOLUTE_SIZE)
    {
        return m_measure;
    }

    int i = m_measure * max;
    return i;
}
