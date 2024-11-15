#pragma once
#include "UIElement.h"

/**
 * A ui component that renders a texture from steam given a steam image handle
 */
class UISteamImage : public UIElement
{
public:
    UISteamImage(const sol::table& table);
    ~UISteamImage() override;
    void Render(const lay_context* ctx) override;

private:
    sol::coroutine m_handle;
    std::optional<Texture2D> m_texture;
    int m_textureHandle;

    static std::unordered_map<int, Texture2D> m_storedTextures;
    static std::unordered_map<int, int> m_storedTexturesRc;
};
