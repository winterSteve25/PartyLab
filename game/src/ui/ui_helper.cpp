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

    Vector2 DrawText(const char* text, float fontSize, Vector2 pos)
    {
        Vector2 size = MeasureTextEx(game_assets::game_font, text, fontSize, 2);
        DrawTextEx(game_assets::game_font, text, pos, fontSize, 2, BLACK);
        return size;
    }

    bool Within(Vector2 x, Vector2 pos, Vector2 size)
    {
        return x.x > pos.x && x.x < pos.x + size.x &&
            x.y > pos.y && x.y < pos.y + size.y;
    }
}
