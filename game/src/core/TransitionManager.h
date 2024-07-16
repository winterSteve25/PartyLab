#pragma once
class TransitionManager
{
public:
    TransitionManager();
    void Render();
    void TransitionTo(int sceneIndex);
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
