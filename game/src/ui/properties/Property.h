#pragma once
#include "ui/styles/Style.h"
class UIElement;

template<typename T>
class Property
{
public:
    explicit Property(UIElement* parent): m_parent(parent)
    {
    }
    
    T Get()
    {
        return m_val;
    }
    
    virtual ~Property() = default;
    virtual void Set(const Style& style, bool doTransition) = 0;
protected:
    T m_val;
    UIElement* m_parent;
};