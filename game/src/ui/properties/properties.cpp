#include "properties.h"

#include "DirectionalProperty.h"

SizeProperty properties::WidthProp()
{
    return SizeProperty(Size(Size::ABSOLUTE, 0), "width", []() { return GetScreenWidth(); });
}

SizeProperty properties::HeightProp()
{
    return SizeProperty(Size(Size::ABSOLUTE, 0), "height", []() { return GetScreenWidth(); });
}

DirectionalProperty properties::MarginLeft()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginLeft", "margin", []() { return GetScreenWidth(); });
}

DirectionalProperty properties::MarginRight()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginRight", "margin", []() { return GetScreenWidth(); });
}

DirectionalProperty properties::MarginTop()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginTop", "margin", []() { return GetScreenHeight(); });
}

DirectionalProperty properties::MarginBottom()
{
    return DirectionalProperty(Size(Size::ABSOLUTE, 0), "marginBottom", "margin", []() { return GetScreenHeight(); });
}

ColorProperty properties::ColorProp(const Color& def)
{
    return ColorProperty(def, "color");
}

ColorProperty properties::BackgroundColorProp(const Color& def)
{
    return ColorProperty(def, "backgroundColor");
}
