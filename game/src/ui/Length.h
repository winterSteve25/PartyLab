#pragma once
#include "sol/sol.hpp"
#define PERCENT_SIZE 1
#define ABSOLUTE_SIZE 2

struct Length
{
public:
    Length();
    Length(const sol::object& size);
    Length(int type, float measure);
    int Eval(int max) const;
    
private:
    int m_type;
    float m_measure;
};