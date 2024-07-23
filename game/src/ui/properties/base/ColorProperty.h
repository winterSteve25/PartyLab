#pragma once
#include "Property.h"
#include "raylib.h"

class ColorProperty : public Property<Color>
{
public:
    ColorProperty(const Color& defaultValue, const std::string& key);
    void Set(const Style& style, bool doTransition) override;
};
