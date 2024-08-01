#pragma once
#include <functional>

#include "sol/sol.hpp"

/**
 * Accessed via Core::INSTANCE->transitionManager
 * Accessed via require("api.core").transitionTo() in lua
 * Can be used to transition to a different scene
 */
class TransitionManager
{
public:
    TransitionManager();
    void Render();
    void TransitionTo(int sceneIndex, const sol::optional<sol::protected_function>& callback);
    void TransitionCustom(void(*callback)());
    bool IsInTransition() const;
private:
    static void ExecuteSceneTransition();
    bool m_inTransition;
    bool m_readyToFinishTransition;
    float m_transitionTime;
    float m_totalTransitionTime;
    std::function<void()> m_callback;
};
