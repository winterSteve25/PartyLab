#pragma once
#include <vector>

#include "raylib.h"
#include "Scene.h"

class Core
{
public:
    Core(int defaultScene);
    void Render(Camera2D& camera);
    void Update();
    void TransitionTo(int sceneIndices);
    void AddScene(Scene* scene);

    static Core* INSTANCE;
    bool shouldExit;

private:
    void Transition();
    
    std::vector<Scene*> m_scenes;
    int m_activesScene;
    int m_loadingScene;
    
    bool m_inTransition;
    bool m_readyToFinishTransition;
    float m_transitionTime;
    float m_totalTransitionTime;
};