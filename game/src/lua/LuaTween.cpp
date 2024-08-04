#include "LuaTween.h"

#include "lua_utils.h"

const std::string LuaTween::error_msg = "Failed to run easing function";
LuaTween::LuaTween(const float from, const float to, const float duration, const sol::protected_function& ease):
    m_t(0),
    m_from(from),
    m_to(to),
    m_duration(duration),
    m_forward(true),
    m_started(false),
    m_ease(ease)
{
}

void LuaTween::Update()
{
    if (!m_started) return;
    if (m_forward ? m_t > 1 : m_t < 0)
    {
        return;
    }

    const float dt = GetFrameTime();
    const float mul = m_forward ? 1 : -1;
    m_t += dt / m_duration * mul;

    if (m_t > 1)
    {
        m_t = 1;
    }
    else if (m_t < 0)
    {
        m_t = 0;
    }
}

void LuaTween::RunForward()
{
    m_forward = true;
    m_started = true;
}

void LuaTween::RunBackward()
{
    m_forward = false;
    m_started = true;
}

void LuaTween::Reset()
{
    m_t = 0;
    m_started = false;
}

float LuaTween::Get() const
{
    const float t = lua_utils::UnwrapResult<float>(m_ease(m_t), error_msg);
    return m_from + (m_to - m_from) * t;
}
