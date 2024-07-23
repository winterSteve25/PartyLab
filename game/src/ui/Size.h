#pragma once
#include "sol/sol.hpp"

struct Size
{
public:
    enum SizeType
    {
        PERCENT,
        ABSOLUTE,
    };

    Size();
    Size(const sol::object& size);
    Size(SizeType type, float measure);
    int Eval(int max) const;
public:
    SizeType m_type;
    float m_measure;
};