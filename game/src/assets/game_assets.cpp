#include "game_assets.h"

namespace game_assets
{
    void LoadAssets()
    {
        GAME_FONT = LoadFontEx("resources/BebasNeue_Regular.ttf", 256, 0, 0);
        SetTextureFilter(GAME_FONT.texture, TEXTURE_FILTER_BILINEAR);
    }
}
