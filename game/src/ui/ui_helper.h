#pragma once

#include "raylib.h"

namespace ui_helper
{
    Vector2 DrawText(const char* text, float fontSize, Vector2 pos, Color color);
    Vector2 MeasureText(const char* text, float fontSize);
    Vector2 GetCenter(Vector2 size, Vector2 totalSize);
    bool Within(Vector2 x, Vector2 pos, Vector2 size);
}
