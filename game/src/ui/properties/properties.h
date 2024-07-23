#pragma once
#include "DirectionalProperty.h"
#include "base/ColorProperty.h"
#include "base/SizeProperty.h"

namespace properties
{
    SizeProperty WidthProp();
    SizeProperty HeightProp();
    DirectionalProperty MarginLeft();
    DirectionalProperty MarginRight();
    DirectionalProperty MarginTop();
    DirectionalProperty MarginBottom();
    ColorProperty ColorProp(const Color& def);
    ColorProperty BackgroundColorProp(const Color& def);
}
