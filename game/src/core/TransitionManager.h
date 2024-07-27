#pragma once

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
    void TransitionTo(int sceneIndex);
    
    /**
     * Instead of transitioning to a different scene, a callback will be called during the black screen
     * @param callback the callback that will be called during the black screen
     */
    void TransitionCustom(void(*callback)());
    bool IsInTransition() const;
private:
    static void ExecuteSceneTransition();
    bool m_inTransition;
    bool m_readyToFinishTransition;
    float m_transitionTime;
    float m_totalTransitionTime;
    void(*m_callback)();
};
