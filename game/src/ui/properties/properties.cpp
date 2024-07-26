#include "properties.h"

#include "DirectionalProperty.h"

static int ScreenWidth()
{
    return GetScreenWidth();
}

static int ScreenHeight()
{
    return GetScreenHeight();
}

SizeProperty properties::WidthProp()
{
    return SizeProperty(Size(Size::ABSOLUTE, 0), "width", ScreenWidth);
}

SizeProperty properties::HeightProp()
{
    return SizeProperty(Size(Size::ABSOLUTE, 0), "height", ScreenHeight);
}

DirectionalProperty properties::MarginLeft()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginLeft", "margin", ScreenWidth);
}

DirectionalProperty properties::MarginRight()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginRight", "margin", ScreenWidth);
}

DirectionalProperty properties::MarginTop()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginTop", "margin", ScreenHeight);
}

DirectionalProperty properties::MarginBottom()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginBottom", "margin", ScreenHeight);
}

ColorProperty properties::ColorProp(const Color& def)
{
    return ColorProperty(def, "color");
}

ColorProperty properties::BackgroundColorProp(const Color& def)
{
    return ColorProperty(def, "backgroundColor");
}
