#pragma once
#include <vector>

#include "raylib.h"
#include "Scene.h"

class Core
{
public:
    Core();
    void Render(Camera2D& camera);
    void Update();
    void TransitionTo(int sceneIndices[]);
    void AddScene(Scene* scene);

private:
    void Transition();
    
    std::vector<Scene*> m_scenes;
    std::vector<int> m_activesScenes;
    std::vector<int> m_overlayScenes;
    std::vector<int> m_loadingScenes;
    
    bool m_inTransition;
    bool m_readyToFinishTransition;
    float m_transitionTime;
    float m_totalTransitionTime;
};