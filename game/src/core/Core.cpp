#include "Core.h"
#include "raylib.h"
#include "assets/game_assets.h"
#include "lua/LuaConstants.h"
#include "lua/LuaScene.h"

Core* Core::INSTANCE = nullptr;

Core::Core(int defaultScene) :
    shouldExit(false),
    m_luaSceneStartIdx(-1),
    m_activesScene(defaultScene),
    m_loadingScene(-1)
{
    Core::INSTANCE = this;
    game_assets::LoadAssets();

    lua.LoadMods();
    AddLuaScenes();

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

void Core::AddLuaScenes()
{
    m_luaSceneStartIdx = m_scenes.size();
    lua.BroadcastEvent<std::function<size_t(const sol::table&)>>(GAME_EVENT_ADD_SCENES, [this](sol::state*)
    {
        return [this](const sol::table& scene)
        {
            TraceLog(LOG_INFO, "Adding custom lua scene");
            sol::object o = scene["load"];
            return this->AddScene(new LuaScene(
                scene["render"],
                scene["renderOverlay"],
                scene["update"],
                scene["load"],
                scene["cleanup"],
                scene["ui"]
            ));
        };
    });
}

void Core::ReloadLua()
{
    TraceLog(LOG_INFO, "Reloading lua");

    floatTweenManager.DeleteAll();
    colorTweenManager.DeleteAll();
    vec2TweenManager.DeleteAll();
    
    for (int i = m_luaSceneStartIdx; i < m_scenes.size(); i++)
    {
        Scene* scene = m_scenes[i];
        scene->Cleanup();
        delete scene;
        m_scenes.erase(m_scenes.begin() + i);
        i--;
    }

    lua.UnloadMods();
    lua.LoadMods();

    AddLuaScenes();

    if (m_activesScene >= m_scenes.size())
    {
        m_activesScene = !m_scenes.empty() ? 0 : -1;
    }

    m_scenes[m_activesScene]->Load();
}

void Core::Render(Camera2D& camera)
{
    ClearBackground(game_assets::BACKGROUND_COLOR);
    
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
    networkManager.HandleMessages();
    if (transitionManager.IsInTransition()) return;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R))
    {
        ReloadLua();
    }

    floatTweenManager.Tick();
    vec2TweenManager.Tick();
    colorTweenManager.Tick();
    if (m_activesScene < 0) return;
    m_scenes[m_activesScene]->Update();
}
