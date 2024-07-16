#include "Core.h"
#include "raylib.h"
#include "assets/game_assets.h"
#include "scenes/BrowseLobbiesScene.h"
#include "scenes/GameLobbyScene.h"
#include "scenes/MainMenuScene.h"

Core* Core::INSTANCE = nullptr;

Core::Core(int defaultScene) :
    shouldExit(false),
    m_activesScene(defaultScene),
    m_loadingScene(-1)
{
    Core::INSTANCE = this;
    game_assets::LoadAssets();
    AddScenes();
    if (m_activesScene < 0) return;
    m_scenes[m_activesScene]->Load();
}

Core::~Core() = default;

size_t Core::AddScene(Scene* scene)
{
    size_t i = m_scenes.size();
    m_scenes.push_back(scene);
    return i;
}

void Core::AddScenes()
{
    AddScene(new MainMenuScene());
    AddScene(new GameLobbyScene());
    AddScene(new BrowseLobbiesScene());
}

void Core::Render(Camera2D& camera)
{
    ClearBackground(game_assets::background_color);
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
    transitionManager.Render();
}

void Core::Update()
{
    if (transitionManager.IsInTransition()) return;
    floatTweenManager.Tick();
    vec2TweenManager.Tick();
    colorTweenManager.Tick();
    if (m_activesScene < 0) return;
    m_scenes[m_activesScene]->Update();
}
