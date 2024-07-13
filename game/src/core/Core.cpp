#include "Core.h"
#include <string>
#include "raylib.h"

Core::Core() :
    m_inTransition(false),
    m_readyToFinishTransition(false),
    m_transitionTime(0),
    m_totalTransitionTime(1.5f)
{
    m_scenes = std::vector<Scene*>();
    m_overlayScenes = std::vector<int>();
    m_activesScenes = std::vector<int>();
    m_activesScenes.reserve(3);
    m_loadingScenes = std::vector<int>();
}

void Core::AddScene(Scene* scene)
{
    m_scenes.push_back(scene);
}

void Core::Render(Camera2D& camera)
{
    ClearBackground(WHITE);
    Transition();

    for (int i : m_overlayScenes)
    {
        m_scenes[i]->Render(camera);
    }

    BeginMode2D(camera);

    for (int i : m_activesScenes)
    {
        m_scenes[i]->Render(camera);
    }

    EndMode2D();
}

void Core::Update()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_inTransition = true;
        m_transitionTime = 0;
    }

    for (int i : m_overlayScenes)
    {
        m_scenes[i]->Update();
    }

    for (int i : m_activesScenes)
    {
        m_scenes[i]->Update();
    }
}

void Core::TransitionTo(int sceneIndices[])
{
    bool any = false;

    for (int s : sceneIndices)
    {
        if (std::find(m_activesScenes.begin(), m_activesScenes.end(), s) != m_activesScenes.end()
            || std::find(m_overlayScenes.begin(), m_overlayScenes.end(), s) != m_overlayScenes.end())
        {
            continue;
        }
        
        m_loadingScenes.push_back(s);
        any = true;
    }

    if (!any) return;

    // should not happen but just in case
    // if occurs it might act funny
    if (!m_inTransition)
    {
        m_inTransition = true;
        m_transitionTime = 0;
    }
}

void RenderTransition(float percentage)
{
    DrawRectangle(0, percentage * GetScreenHeight(), GetScreenWidth(), GetScreenHeight(), BLACK);
}

float Ease(float t)
{
    return 1 - pow(1 - t, 5);
}

void Core::Transition()
{
    if (!m_inTransition) return;
    m_transitionTime += GetFrameTime();

    if (m_transitionTime >= 0)
    {
        float t = Ease(m_transitionTime / m_totalTransitionTime);
        RenderTransition(m_readyToFinishTransition ? -t : (1 - t));
    } else
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    }

    if (m_transitionTime < m_totalTransitionTime) return;
    if (m_readyToFinishTransition)
    {
        m_readyToFinishTransition = false;
        m_transitionTime = 0;
        m_inTransition = false;
    }
    else
    {
        for (int scene : m_activesScenes)
        {
            m_scenes[scene]->Cleanup();
        }

        m_activesScenes.clear();
        
        for (int scene : m_loadingScenes)
        {
            m_scenes[scene]->Load();
            m_activesScenes.push_back(scene);
        }

        m_loadingScenes.clear();
        m_readyToFinishTransition = true;
        m_transitionTime = -1; // delay before actually starts finishing
    }
}
