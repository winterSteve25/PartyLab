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
    void TransitionTo(int sceneIndex);

private:
    std::vector<Scene> m_scenes;
    std::vector<int> m_activesScenes;
    bool m_inTransition;
    float m_transitionTime;
};