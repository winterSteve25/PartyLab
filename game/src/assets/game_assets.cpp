#include "game_assets.h"

namespace game_assets
{
    void LoadAssets()
    {
        game_font = LoadFontEx("resources/BebasNeue_Regular.ttf", 256, 0, 0);
        SetTextureFilter(game_font.texture, TEXTURE_FILTER_BILINEAR);
    }
}
