#pragma once

/**
 * Basically a marker on a type to mark the destruction of the contained type
 * @tparam T Type to be managed, should be a pointer of some type
 */
template <typename T>
class Managed
{
public:
    Managed(const T& obj): m_obj(obj), m_killed(false)
    {
    }

    void Kill()
    {
        m_killed = true;
    }

    bool IsKilled()
    {
        return m_killed;
    }
    
    T Value()
    {
        return m_obj;
    }
private:
    T m_obj;
    bool m_killed;
};
