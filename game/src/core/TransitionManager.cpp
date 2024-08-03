#include "TransitionManager.h"
#include "Core.h"

void TransitionManager::ExecuteSceneTransition()
{
    Core::INSTANCE->colorTweenManager.DeleteAll();
    Core::INSTANCE->floatTweenManager.DeleteAll();
    Core::INSTANCE->vec2TweenManager.DeleteAll();

     Core::INSTANCE->m_scenes[Core::INSTANCE->m_activesScene]->Cleanup();
     Core::INSTANCE->m_scenes[Core::INSTANCE->m_loadingScene]->Load();
     Core::INSTANCE->m_activesScene = Core::INSTANCE->m_loadingScene;
     Core::INSTANCE->m_loadingScene = -1;
}

TransitionManager::TransitionManager():
    m_inTransition(false),
    m_readyToFinishTransition(false),
    m_transitionTime(0),
    m_totalTransitionTime(0.75f)
{
}

void TransitionManager::TransitionTo(int scene, const sol::optional<sol::protected_function>& callback)
{
    if (m_inTransition) return;
    if (Core::INSTANCE->m_activesScene == scene) return;
    if (Core::INSTANCE->m_loadingScene == scene) return;
    if (Core::INSTANCE->m_loadingScene != -1)
    {
        TraceLog(LOG_WARNING, "Trying to load scene when loading another scene already");
        return;
    }

    Core::INSTANCE->m_loadingScene = scene;
    m_inTransition = true;
    m_transitionTime = 0;

    if (callback.has_value())
    {
        const sol::protected_function cbv = callback.value();
        m_callback = [cbv]()
        {
            lua_utils::UnwrapResult(cbv(), "Failed to execute transition callback");
            ExecuteSceneTransition();
        };
    }
    else
    {
        m_callback = ExecuteSceneTransition;
    }
}

void TransitionManager::TransitionCustom(void (*callback)())
{
    if (m_inTransition) return;
    m_inTransition = true;
    m_transitionTime = 0;
    m_callback = callback;
}

bool TransitionManager::IsInTransition() const
{
    return m_inTransition;
}

static void render_transition(float percentage)
{
    DrawRectangle(0, static_cast<int>(GetScreenHeight() * percentage), GetScreenWidth(), GetScreenHeight(), BLACK);
}

static float ease(float t)
{
    return 1.0f - static_cast<float>(pow(1 - t, 5));
}

void TransitionManager::Render()
{
    if (!m_inTransition) return;
    m_transitionTime += GetFrameTime();

    if (m_transitionTime >= 0)
    {
        float t = ease(m_transitionTime / m_totalTransitionTime);
        render_transition(m_readyToFinishTransition ? -t : (1 - t));
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
        m_callback();
        m_readyToFinishTransition = true;
        m_transitionTime = 0.0f;
        EndDrawing(); // used to reset frame time so the outro animation wont be cut if loading takes long
        BeginDrawing();
    }
}
