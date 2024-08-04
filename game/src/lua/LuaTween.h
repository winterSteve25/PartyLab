#pragma once
#include "sol/sol.hpp"

class LuaTween
{
public:
    LuaTween(float from, float to, float duration, const sol::protected_function& ease);
    void Update();
    void RunForward();
    void RunBackward();
    void Reset();
    float Get() const;
private:
    static const std::string error_msg;
    
    float m_t;
    float m_from;
    float m_to;
    float m_duration;
    
    bool m_forward;
    bool m_started;
    
    sol::protected_function m_ease;
};
