#include "MainMenuScene.h"

#include "raylib.h"
#include "assets/game_assets.h"
#include "core/Core.h"
#include "ui/ui_helper.h"

using namespace ui_helper;

static void JoinGame()
{
    Core::INSTANCE->TransitionTo(1);
}

static void HostGame()
{
    Core::INSTANCE->TransitionTo(2);
}

static void Addons()
{
}

static void Quit()
{
    Core::INSTANCE->shouldExit = true;
}

void MainMenuScene::Load()
{
    m_joinGame = new UIButton(
        AnchorTopLeft({120, 240}),
        48,
        "Join Game",
        JoinGame
    );
    
    m_hostGame = new UIButton(
        AnchorTopLeft({120, 300}),
        48,
        "Host Game",
        JoinGame
    );
}

void MainMenuScene::Cleanup()
{
    delete m_joinGame;
    delete m_hostGame;
    // delete m_addons;
    // delete m_quit;
}

void MainMenuScene::RenderOverlay()
{
    DrawTextEx(game_assets::game_font, "Party Lab", ui_helper::AnchorTopLeft({100, 100}), 128, 2, BLACK);
    m_joinGame->Render();
    m_hostGame->Render();
}
