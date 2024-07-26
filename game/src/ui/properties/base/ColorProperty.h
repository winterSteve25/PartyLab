#pragma once
#include "Property.h"
#include "raylib.h"
#include "TweenManagingProperty.h"
#include "tweeny/tweeny.h"

class ColorProperty : public TweenManagingProperty<Color, unsigned char, unsigned char, unsigned char, unsigned char>
{
public:
    ColorProperty(const Color& defaultValue, const std::string& key);
    void Set(const Style& style, bool doTransition) override;
};
