#pragma once
#include "Property.h"
#include "TweenManagingProperty.h"
#include "tweeny/tweeny.h"

class FloatProperty : public TweenManagingProperty<float, float>
{
public:
    FloatProperty(float defaultValue, const std::string& key);
    void Set(const Style& style, bool doTransition) override;
protected:
    virtual void OnSet();
    virtual float GetNewValue(const Style& style);
};
