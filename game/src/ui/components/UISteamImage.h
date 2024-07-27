#pragma once
#include "UIElement.h"

/**
 * A ui component that renders a texture from steam given a steam image handle
 */
class UISteamImage : UIElement
{
public:
    UISteamImage(const sol::table& table);
    void Render(const lay_context* ctx) override;

private:
    sol::coroutine m_handle;
    std::optional<RenderTexture2D> m_texture;
};
