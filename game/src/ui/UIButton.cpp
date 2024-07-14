#include "UIButton.h"

#include "ui_helper.h"
#include "assets/game_assets.h"
#include "tweeny/tweeny.h"

UIButton::UIButton(Vector2 pos, float font_size, const char* text, void (*onClicked)()):
    UIElement(pos, MeasureTextEx(game_assets::game_font, text, font_size, 2)),
    m_fontSize(font_size),
    m_text(text),
    m_onClicked(onClicked)
{
    m_hoveredSize = MeasureTextEx(game_assets::game_font, text, font_size * 1.5f, 2);
}

void UIButton::Render()
{
    UIElement::Render();
    DrawTextEx(game_assets::game_font, m_text, m_pos, m_fontSize, 2, BLACK);
}

void UIButton::OnClick()
{
    m_onClicked();
}

void UIButton::OnHover()
{
    auto tween = tweeny::from(1.0f)
        .to(1.25f)
        .during();

    tween.step()
}
