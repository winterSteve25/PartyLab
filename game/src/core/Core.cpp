#include "Core.h"
#include "raylib.h"

Core::Core()
{
    m_scenes = std::vector<Scene>();
    m_activesScenes = std::vector<int>(5);
}

void Core::Render(Camera2D& camera)
{
    ClearBackground(WHITE);

    for (int i : m_activesScenes)
    {
        m_scenes[i].Render(camera);
    }
}

void Core::Update()
{
    for (int i : m_activesScenes)
    {
        m_scenes[i].Update();
    }
}

void Core::TransitionTo(int sceneIndex)
{
    if (!m_inTransition)
    {
        m_inTransition = true;
    }
    
    m_activesScenes.clear();
}
