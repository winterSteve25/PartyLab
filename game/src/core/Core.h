#pragma once
#include <vector>

#include "raylib.h"
#include "Scene.h"
#include "TransitionManager.h"
#include "SimpleTweenManager.h"
#include "lua/LuaHook.h"
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
    void AddScenes();
    
    bool shouldExit;
    TransitionManager transitionManager;
    NetworkManager networkManager;
    LuaHook lua;
    
    SimpleTweenManager<float> floatTweenManager;
    SimpleTweenManager<float, float> vec2TweenManager;
    SimpleTweenManager<unsigned char, unsigned char, unsigned char, unsigned char> colorTweenManager;
private:
    std::vector<Scene*> m_scenes;
    int m_activesScene;
    int m_loadingScene;
    friend TransitionManager;
};
