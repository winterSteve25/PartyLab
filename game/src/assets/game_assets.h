#pragma once
#include "raylib.h"

namespace game_assets
{
    inline Font game_font;
    inline Color text_color = { 22, 22, 22, 255 };
    inline Color accent_color = { 8, 88, 182, 255 };
    inline Color foreground_color = { 0, 179, 255, 51 };
    inline Color middleground_color = { 8, 88, 182, 255 };
    inline Color background_color = { 243, 243, 243, 255 };
    
    void LoadAssets();
}
