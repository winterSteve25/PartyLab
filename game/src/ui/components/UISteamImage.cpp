#include "UISteamImage.h"

UISteamImage::UISteamImage(const sol::table& table):
    UIElement(table),
    m_handle(table["image"]),
    m_texture(std::optional<RenderTexture2D>(std::nullopt))
{
}

void UISteamImage::Render(const lay_context* ctx)
{
    
}
