#include "Core.h"
#include "raylib.h"
#include "TestScene.h"
#include "assets/game_assets.h"
#include "lua/LuaConstants.h"
#include "lua/LuaScene.h"

Core* Core::INSTANCE = nullptr;

Core::Core(int defaultScene) :
    shouldExit(false),
    m_activesScene(defaultScene),
    m_loadingScene(-1)
{
    Core::INSTANCE = this;
    lua.LoadMods();
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
    AddScene(new TestScene);
    lua.BroadcastEvent<std::function<size_t(const sol::table&)>>(GAME_EVENT_ADD_SCENES, [this](sol::state*)
    {
        return [this](const sol::table& scene)
        {
            TraceLog(LOG_INFO, "Custom lua scene added");
            return this->AddScene(new LuaScene(
                scene[SCENE_PROP_RENDER],
                scene[SCENE_PROP_RENDER_OVERLAY],
                scene[SCENE_PROP_UPDATE],
                scene[SCENE_PROP_LOAD],
                scene[SCENE_PROP_CLEANUP],
                scene[SCENE_PROP_UI]
            ));
        };
    });
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
