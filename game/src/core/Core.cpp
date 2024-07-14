#include "Core.h"
#include <string>
#include "raylib.h"
#include "assets/game_assets.h"
#include "scenes/MainMenuScene.h"

Core* Core::INSTANCE = nullptr;

Core::Core(int defaultScene) :
    shouldExit(false),
    m_activesScene(defaultScene),
    m_loadingScene(-1),
    m_inTransition(false),
    m_readyToFinishTransition(false),
    m_transitionTime(0),
    m_totalTransitionTime(1.5f)
{
    Core::INSTANCE = this;
    game_assets::LoadAssets();
    
    AddScene(new MainMenuScene());

    if (m_activesScene < 0) return;
    m_scenes[m_activesScene]->Load();
}

void Core::AddScene(Scene* scene)
{
    m_scenes.push_back(scene);
}

void Core::Render(Camera2D& camera)
{
    ClearBackground(WHITE);
    Transition();

    if (m_activesScene >= 0)
    {
        m_scenes[m_activesScene]->RenderOverlay();
    }

    BeginMode2D(camera);

    if (m_activesScene >= 0)
    {
        m_scenes[m_activesScene]->Render();
    }

    EndMode2D();
}

void Core::Update()
{
    if (m_activesScene < 0) return;
    m_scenes[m_activesScene]->Update();
}

void Core::TransitionTo(int scene)
{
    if (m_activesScene == scene) return;
    if (m_loadingScene == scene) return;
    if (m_loadingScene != -1)
    {
        TraceLog(LOG_WARNING, "Trying to load scene when loading another scene already");
        return;
    }

    m_loadingScene = scene;
    m_inTransition = true;
    m_transitionTime = 0;
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
    }
    else
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    }

    if (m_transitionTime < m_totalTransitionTime) return;
    if (m_readyToFinishTransition)
    {
        // finished transition
        m_readyToFinishTransition = false;
        m_transitionTime = 0;
        m_inTransition = false;
    }
    else
    {
        // finished transition intro, begin actual loading
        m_scenes[m_activesScene]->Cleanup();
        m_scenes[m_loadingScene]->Load();
        m_activesScene = m_loadingScene;
        m_loadingScene = -1;
        m_readyToFinishTransition = true;
        m_transitionTime = -1; // delay before actually starts finishing
    }
}
