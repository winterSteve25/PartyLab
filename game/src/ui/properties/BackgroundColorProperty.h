#pragma once
#include "ColorProperty.h"

class BackgroundColorProperty : public ColorProperty
{
public:
    BackgroundColorProperty(UIElement* parent);
    void Set(const Style& style, bool doTransition) override;
};
