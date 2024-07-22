#pragma once
#include "Property.h"

class ColorProperty : public Property<Color>
{
public:
    ColorProperty(UIElement* parent);
    void Set(const Style& style, bool doTransition) override;
};
