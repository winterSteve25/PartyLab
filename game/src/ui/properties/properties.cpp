#include "properties.h"

#include "DirectionalProperty.h"

LengthProperty properties::WidthProp()
{
    return LengthProperty(Length(ABSOLUTE_SIZE, 0), "width", true);
}

LengthProperty properties::HeightProp()
{
    return LengthProperty(Length(ABSOLUTE_SIZE, 0), "height", false);
}

DirectionalProperty properties::MarginLeft()
{
    return DirectionalProperty(Length(ABSOLUTE_SIZE, 0), "marginLeft", "margin", true);
}

DirectionalProperty properties::MarginRight()
{
    return DirectionalProperty(Length(ABSOLUTE_SIZE, 0), "marginRight", "margin", true);
}

DirectionalProperty properties::MarginTop()
{
    return DirectionalProperty(Length(ABSOLUTE_SIZE, 0), "marginTop", "margin", false);
}

DirectionalProperty properties::MarginBottom()
{
    return DirectionalProperty(Length(ABSOLUTE_SIZE, 0), "marginBottom", "margin", false);
}

ColorProperty properties::ColorProp(const Color& def)
{
    return ColorProperty(def, "color");
}

ColorProperty properties::BackgroundColorProp(const Color& def)
{
    return ColorProperty(def, "backgroundColor");
}

BooleanProperty properties::CanClipProp()
{
    return BooleanProperty(false, "clip");
}

BooleanProperty properties::Scrollable()
{
    return BooleanProperty(false, "scrollable");
}
