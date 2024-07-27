#pragma once

#include "ui/Style.h"

class UIElement;

/**
 * Represents a style properties found in a UI component
 * Different components may support different properties, refer to their specific implementations
 * @tparam T the property's type, for example, fontSize uses float
 */
template <typename T>
class Property
{
public:
    virtual ~Property() = default;
    explicit Property(const T& defaultValue, const std::string& key):
        m_defaultValue(defaultValue),
        m_key(key)
    {
    }

    T Get()
    {
        return m_val;
    }

    virtual void Set(const Style& style, bool doTransition) = 0;
protected:
    T m_val;
    T m_defaultValue;
    std::string m_key;
};
