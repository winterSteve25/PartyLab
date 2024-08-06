#pragma once
#include "DirectionalProperty.h"
#include "base/BooleanProperty.h"
#include "base/ColorProperty.h"
#include "base/LengthProperty.h"

namespace properties
{
    LengthProperty WidthProp();
    LengthProperty HeightProp();
    DirectionalProperty MarginLeft();
    DirectionalProperty MarginRight();
    DirectionalProperty MarginTop();
    DirectionalProperty MarginBottom();
    ColorProperty ColorProp(const Color& def);
    ColorProperty BackgroundColorProp(const Color& def);
    BooleanProperty CanClipProp();
    BooleanProperty Scrollable();
    DirectionalProperty PaddingLeft();
    DirectionalProperty PaddingRight();
    DirectionalProperty PaddingTop();
    DirectionalProperty PaddingBottom();
}
