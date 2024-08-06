#include "game_assets.h"

#include <cstdint>

namespace game_assets
{
    void LoadAssets()
    {
        const int baseSize = 64;
        int fileSize = 0;
        unsigned char* fileData = LoadFileData("resources/BebasNeue_Regular.ttf", &fileSize);

        GAME_FONT = {};
        GAME_FONT.baseSize = baseSize;
        GAME_FONT.glyphCount = 95;
        GAME_FONT.glyphs = LoadFontData(fileData, fileSize, baseSize, NULL, 0, FONT_SDF);

        Image atlas = GenImageFontAtlas(GAME_FONT.glyphs, &GAME_FONT.recs, 95, baseSize, 0, 1);
        GAME_FONT.texture = LoadTextureFromImage(atlas);
        UnloadImage(atlas);
        UnloadFileData(fileData);

        SetTextureFilter(GAME_FONT.texture, TEXTURE_FILTER_BILINEAR);
        SDF_SHADER = LoadShader("resources/shaders/sdf_vs.glsl", "resources/shaders/sdf_fs.glsl");
    }

    void UnloadAssets()
    {
        UnloadFont(GAME_FONT);
        UnloadShader(SDF_SHADER);
    }
}
