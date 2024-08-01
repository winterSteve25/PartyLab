#pragma once
#include "raylib.h"
#include "base/FloatProperty.h"


/**
 * Conforms the aspect ratio of the ui element in the form of w:h
 */
class RatioProperty : public FloatProperty
{
public:
    RatioProperty();
    Vector2 Apply(Vector2 size) const;
};
