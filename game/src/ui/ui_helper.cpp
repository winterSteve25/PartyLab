#include "ui_helper.h"

#include "assets/game_assets.h"

namespace ui_helper
{
    Vector2 DrawText(const char* text, float fontSize, Vector2 pos, Color color)
    {
        Vector2 size = MeasureText(text, fontSize);
        DrawTextEx(game_assets::GAME_FONT, text, pos, fontSize, 1, color);
        return size;
    }

    Vector2 MeasureText(const char* text, float fontSize)
    {
        Vector2 s = MeasureTextEx(game_assets::GAME_FONT, text, fontSize, 1);
        s.y -= s.y * 0.38f;
        return s;
    }

    Vector2 GetCenter(Vector2 size, Vector2 totalSize)
    {
        return {
            (totalSize.x - size.x) / 2,
            (totalSize.y - size.y) / 2
        };
    }

    bool Within(Vector2 x, Vector2 pos, Vector2 size)
    {
        return x.x > pos.x && x.x < pos.x + size.x &&
            x.y > pos.y && x.y < pos.y + size.y;
    }
}
