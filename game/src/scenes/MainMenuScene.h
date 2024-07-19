#pragma once
#include "raylib.h"
#include "assets/game_assets.h"
#include "core/Core.h"
#include "core/Scene.h"
#include "ui/ui_helper.h"
#include "ui/UIButton.h"

using namespace ui_helper;

static void JoinGame()
{
    Core::INSTANCE->transitionManager.TransitionTo(2);
}

static void HostGame()
{
    Core::INSTANCE->networkManager.HostLobby();
    Core::INSTANCE->transitionManager.TransitionTo(1);
}

static void Quit()
{
    Core::INSTANCE->shouldExit = true;
}

class MainMenuScene : public Scene
{
public:
    void RenderOverlay() override
    {
        DrawText("Party", 128, AnchorTopLeft({100, 100}), game_assets::text_color);
        DrawText("Lab", 128, AnchorTopLeft({320, 100}), game_assets::accent_color);
        Scene::RenderOverlay();
    }

    void Load() override
    {
        UIButton* joinGame = new UIButton(
            AnchorTopLeft({120, 240}),
            64,
            "Join Game",
            JoinGame
        );

        UIButton* hostGame = new UIButton(
            AnchorTopLeft({120, 320}),
            64,
            "Host Game",
            HostGame
        );

        UIButton* quit = new UIButton(
            AnchorTopLeft({120, 400}),
            64,
            "Quit",
            Quit
        );

        AddRenderable(joinGame);
        AddRenderable(hostGame);
        AddRenderable(quit);
    }
};
