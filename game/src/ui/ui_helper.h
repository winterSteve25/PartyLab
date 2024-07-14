#pragma once
#include "raylib.h"

namespace ui_helper
{
    Vector2 AnchorTopLeft(Vector2 pos);
    Vector2 AnchorTopRight(Vector2 pos);
    Vector2 AnchorBtmLeft(Vector2 pos);
    Vector2 AnchorBtmRight(Vector2 pos);
    bool DrawButton(Vector2 pos, const char* text, float fontSize, Color color, Vector2& outputSize);
    bool Within(Vector2 x, Vector2 pos, Vector2 size);
}
