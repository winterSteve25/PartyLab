#include "ui_helper.h"

#include "assets/game_assets.h"

namespace ui_helper
{
    Vector2 AnchorTopLeft(Vector2 pos)
    {
        return pos;
    }

    Vector2 AnchorTopRight(Vector2 pos)
    {
        return { GetScreenWidth() - pos.x, pos.y };
    }

    Vector2 AnchorBtmRight(Vector2 pos)
    {
        return { GetScreenWidth() - pos.x, GetScreenHeight() - pos.y };
    }
    
    Vector2 AnchorBtmLeft(Vector2 pos)
    {
        return { pos.x, GetScreenHeight() - pos.y };
    }

    Vector2 DrawText(const char* text, float fontSize, Vector2 pos, Color color)
    {
        Vector2 size = MeasureText(text, fontSize);
        DrawTextEx(game_assets::game_font, text, pos, fontSize, 2, color);
        return size;
    }

    Vector2 MeasureText(const char* text, float fontSize)
    {
        return MeasureTextEx(game_assets::game_font, text, fontSize, 2);
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
