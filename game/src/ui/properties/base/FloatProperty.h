#pragma once
#include "Property.h"

class FloatProperty : public Property<float>
{
public:
    FloatProperty(float defaultValue, const std::string& key);
    void Set(const Style& style, bool doTransition) override;
protected:
    virtual void OnSet();
    virtual float GetNewValue(const Style& style);
};
