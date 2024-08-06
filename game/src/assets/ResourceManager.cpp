#include "ResourceManager.h"

ResourceManager::ResourceManager():
    m_next(0)
{
}

int ResourceManager::LoadTex(const std::string& file)
{
    const Texture2D texture = LoadTexture(file.c_str());
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    m_loadedTextures[m_next] = texture;
    return m_next++;
};

void ResourceManager::UnloadTex(int handle)
{
    Texture2D tex = m_loadedTextures[handle];
    UnloadTexture(tex);
    m_loadedTextures.erase(handle);
}

Texture2D ResourceManager::GetTex(int handle)
{
    return m_loadedTextures[handle];
}
