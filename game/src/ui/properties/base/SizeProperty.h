#pragma once
#include "Property.h"
#include "ui/Size.h"

class SizeProperty
{
public:
    SizeProperty(const Size& defaultValue, const std::string& key, const std::function<int()>& max);
    virtual void Set(const Style& style, bool doTransition);
    float Get() const;
    void Override(float val);

protected:
    float m_evaled;
    float m_override;
    bool m_overriden;
    
    std::function<int()> m_max;
    std::string m_key;
    Size m_default;
};
