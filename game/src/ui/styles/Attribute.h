#pragma once

template <typename T>
class Attribute
{
public:
    T val;
    bool isDefault;

    Attribute(): isDefault(true)
    {
    }
    
    Attribute(const T& val, bool is_default)
        : val(val),
          isDefault(is_default)
    {
    }

    operator T() const
    {
        return val;
    }

    void InheritFrom(Attribute<T> other)
    {
        if (!isDefault) return;
        val = other.val;
    }
};
