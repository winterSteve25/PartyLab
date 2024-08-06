#pragma once
#include <string>
#include <unordered_map>

#include "raylib.h"

class ResourceManager
{
public:
    ResourceManager();
    int LoadTex(const std::string& file);
    void UnloadTex(int handle);
    Texture2D GetTex(int handle);
private:
    std::unordered_map<int, Texture2D> m_loadedTextures;

    int m_next;
};
