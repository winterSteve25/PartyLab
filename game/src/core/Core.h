#pragma once
#include <vector>

#include "raylib.h"
#include "Scene.h"
#include "TransitionManager.h"
#include "SimpleTweenManager.h"
#include "lua/ModManager.h"
#include "network/NetworkManager.h"

/**
 * Manages the scenes in the game
 */
class Core
{
public:
    static Core* INSTANCE;
    
    Core(int defaultScene);
    ~Core();
    
    void Render(Camera2D& camera);
    void Update();
    
    /**
     * All scenes should be registered at the start of the application 
     * @param scene The scene to be added, the memory will be managed by the core
     * @return the index of the newly added scene, used to transition to the scene
     */
    size_t AddScene(Scene* scene);
    void AddLuaScenes();
    
    bool shouldExit;
    
    TransitionManager transitionManager;
    NetworkManager networkManager;
    ModManager lua;
    
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
