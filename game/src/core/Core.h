#pragma once
#include <vector>

#include "raylib.h"
#include "Scene.h"
#include "TransitionManager.h"
#include "SimpleTweenManager.h"
#include "lua/LuaManager.h"
#include "network/NetworkManager.h"

class Core
{
public:
    static Core* INSTANCE;
    
    Core(int defaultScene);
    ~Core();
    
    void Render(Camera2D& camera);
    void Update();
    size_t AddScene(Scene* scene);
    void AddLuaScenes();
    
    bool shouldExit;
    TransitionManager transitionManager;
    NetworkManager networkManager;
    LuaManager lua;
    
    SimpleTweenManager<float> floatTweenManager;
    SimpleTweenManager<float, float> vec2TweenManager;
    SimpleTweenManager<unsigned char, unsigned char, unsigned char, unsigned char> colorTweenManager;
private:
    void ReloadLua();

    int m_luaSceneStartIdx;
    std::vector<Scene*> m_scenes;
    int m_activesScene;
    int m_loadingScene;
    friend TransitionManager;
};