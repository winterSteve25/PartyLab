#pragma once
#include "raylib.h"

namespace game_assets
{
    inline Font GAME_FONT;
    
    inline Color TEXT_COLOR = { 22, 22, 22, 255 };
    inline Color ACCENT_COLOR = { 8, 88, 182, 255 };
    inline Color FOREGROUND_COLOR = { 0, 179, 255, 51 };
    inline Color MIDDLEGROUND_COLOR = { 8, 88, 182, 255 };
    inline Color BACKGROUND_COLOR = { 243, 243, 243, 255 };
    
    void LoadAssets();
}
