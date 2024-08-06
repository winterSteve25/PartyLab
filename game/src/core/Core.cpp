#include "Core.h"
#include "raylib.h"
#include "assets/game_assets.h"
#include "lua/LuaConstants.h"
#include "lua/LuaScene.h"

Core* Core::INSTANCE = nullptr;

Core::Core(int defaultScene) :
    shouldExit(false),
    m_activesScene(defaultScene),
    m_loadingScene(-1)
{
    INSTANCE = this;
    game_assets::LoadAssets();

    modManager.LoadMods();
    AddLuaScenes();
    networkManager.RegisterPackets();
    AddGameModes();

    if (m_activesScene < 0) return;
    m_scenes[m_activesScene]->Load();
}

Core::~Core() = default;

size_t Core::AddScene(LuaScene* scene)
{
    size_t i = m_scenes.size();
    m_scenes.push_back(scene);
    return i;
}

void Core::AddLuaScenes()
{
    modManager.BroadcastEvent<std::function<uint32_t(const sol::table&)>>(GAME_EVENT_ADD_SCENES, [this](sol::state*)
    {
        return [this](const sol::table& scene)
        {
            TraceLog(LOG_INFO, "Adding custom lua scene");
            return this->AddScene(new LuaScene(
                scene["render"],
                scene["renderOverlay"],
                scene["update"],
                scene["load"],
                scene["cleanup"],
                scene["ui"],
                scene["events"]
            ));
        };
    });
}

void Core::AddGameModes()
{
    modManager.BroadcastEvent<std::function<void(const sol::table&)>>(GAME_EVENT_ADD_GAME_MODES, [this](sol::state*)
    {
        return [this](const sol::table& gamemode)
        {
            const uint32_t idx = static_cast<uint32_t>(gameModes.size());
            TraceLog(LOG_INFO, std::format("Adding custom lua game mode type with id {}", idx).c_str());
            this->gameModes.push_back(gamemode);
        };
    });
}

void Core::ReloadLua()
{
    TraceLog(LOG_INFO, "Reloading lua");

    floatTweenManager.DeleteAll();
    colorTweenManager.DeleteAll();
    vec2TweenManager.DeleteAll();

    for (int i = 0; i < m_scenes.size(); i++)
    {
        LuaScene* scene = m_scenes[i];

        if (i == m_activesScene)
        {
            scene->Cleanup();
        }

        delete scene;
    }

    m_scenes.clear();
    gameModes.clear();
    networkManager.UnregisterPackets();
    modManager.UnloadMods();

    game_assets::UnloadAssets();
    game_assets::LoadAssets();

    modManager.LoadMods();

    AddLuaScenes();
    networkManager.RegisterPackets();
    AddGameModes();

    if (m_activesScene >= m_scenes.size())
    {
        m_activesScene = !m_scenes.empty() ? 0 : -1;
    }

    m_scenes[m_activesScene]->Load();
}

void Core::Render(Camera2D& camera)
{
    ClearBackground(game_assets::BACKGROUND_COLOR);

    BeginMode2D(camera);

    if (m_activesScene >= 0)
    {
        m_scenes[m_activesScene]->Render();
    }

    EndMode2D();

    if (m_activesScene >= 0)
    {
        m_scenes[m_activesScene]->RenderOverlay();
    }

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
