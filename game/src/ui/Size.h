#pragma once
#include "sol/sol.hpp"

struct Size
{
public:
    enum SizeType : uint8_t
    {
        VIEW = 1,
        ABSOLUTE = 2,
    };

    Size();
    Size(const sol::object& size);
    Size(SizeType type, float measure);
    int Eval(int max) const;
    
private:
    SizeType m_type;
    float m_measure;
};