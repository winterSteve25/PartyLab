#pragma once
#include <vector>

#include "utils/TransitionManager.h"
#include "utils/SimpleTweenManager.h"
#include "assets/ResourceManager.h"
#include "lua/LuaScene.h"
#include "lua/ModManager.h"
#include "lua/async/LuaAsyncManager.h"
#include "network/NetworkManager.h"
#include "network/synced/SyncManager.h"

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
    size_t AddScene(LuaScene* scene);
    void AddLuaScenes();
    void AddGameModes();

    /**
     * Broadcasts a scene event to the active scene. This is different from the global events dispatched through ModManager
     * These events are only handled by the active scene
     * @tparam Args Argument types
     * @param event The name of the event
     * @param args The arguments
     */
    template <typename... Args>
    void BroadcastSceneEvent(const std::string& event, const std::function<Args(sol::state_view*)>&... args)
    {
        if (m_activesScene < 0 || m_activesScene > m_scenes.size()) return;
        m_scenes[m_activesScene]->ReceiveEvent<Args...>(event, args...);
    }
    
    bool shouldExit;
    
    TransitionManager transitionManager;
    NetworkManager networkManager;
    ModManager modManager;
    LuaAsyncManager luaAsyncManager;
    SyncManager syncManager;
    ResourceManager resourceManager;
    std::vector<sol::table> gameModes;
    
    SimpleTweenManager<float> floatTweenManager;
    SimpleTweenManager<float, float> vec2TweenManager;
    SimpleTweenManager<unsigned char, unsigned char, unsigned char, unsigned char> colorTweenManager;
private:
    void ReloadLua();

    std::vector<LuaScene*> m_scenes;
    int m_activesScene;
    int m_loadingScene;
    friend TransitionManager;
};
