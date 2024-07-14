#pragma once
#include "layout.h"
#include "core/Scene.h"
#include "ui/UIButton.h"

class MainMenuScene final : public Scene
{
public:
    void RenderOverlay() override;
    void Load() override;
    void Cleanup() override;
private:
    UIButton* m_joinGame;
    UIButton* m_hostGame;
    UIButton* m_addons;
    UIButton* m_quit;
};
