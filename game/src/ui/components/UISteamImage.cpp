#include "UISteamImage.h"

#include <isteamutils.h>

#include "core/Core.h"
#include "lua/lua_utils.h"

static bool BuildImage(Image* image, int handle)
{
    if (handle == -1 || handle == 0) return false;

    uint32 width, height;
    if (!SteamUtils()->GetImageSize(handle, &width, &height)) return false;

    const int size = width * height * 4 * sizeof(char);
    auto buffer = malloc(size); // freed by UnloadImage after this call is returned

    SteamUtils()->GetImageRGBA(handle, static_cast<uint8*>(buffer), size);

    image->width = width;
    image->height = height;
    image->data = buffer;
    image->mipmaps = 1;
    image->format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    return true;
}

UISteamImage::UISteamImage(const sol::table& table):
    UIElement(table),
    m_texture(std::optional<Texture2D>(std::nullopt))
{
    Core::INSTANCE->luaAsyncManager.CallAsync(table["image"], [this](const sol::protected_function_result& result)
    {
        int handle = lua_utils::UnwrapResult<int>(result, "Failed to get image handle");
        Image img;

        if (!BuildImage(&img, handle))
        {
            TraceLog(LOG_WARNING, "Failed to load steam image");
            return;
        }

        m_texture = LoadTextureFromImage(img);
        UnloadImage(img);
    });
}

UISteamImage::~UISteamImage()
{
    if (!m_texture.has_value()) return;
    UnloadTexture(m_texture.value());
}

void UISteamImage::Render(const lay_context* ctx)
{
    if (m_texture.has_value())
    {
        Vector2 pos = GetPos(ctx);
        Vector2 size = GetSize(ctx);
        Texture2D& tex = m_texture.value();

        DrawTexturePro(
            tex,
            Rectangle{0, 0, static_cast<float>(tex.width), static_cast<float>(tex.height)},
            Rectangle{pos.x, pos.y, size.x, size.y},
            {0, 0},
            0,
            WHITE
        );
    }

    UIElement::Render(ctx);
}
