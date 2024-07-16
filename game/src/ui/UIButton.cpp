#include "UIButton.h"

#include "ui_helper.h"
#include "assets/game_assets.h"
#include "core/Core.h"
#include "tweeny/tweeny.h"

UIButton::UIButton(Vector2 pos, float font_size, const char* text, void (*onClicked)()):
    UIElement(pos, MeasureTextEx(game_assets::game_font, text, font_size, 2)),
    m_fontSize(font_size),
    m_actualFontSize(font_size),
    m_color(game_assets::text_color),
    m_text(text),
    m_onClicked(onClicked)
{
    m_hoveredSize = MeasureTextEx(game_assets::game_font, text, font_size * 1.5f, 2);
}

void UIButton::RenderOverlay()
{
    DrawTextEx(game_assets::game_font, m_text, m_pos, m_actualFontSize, 2, m_color);
}

void UIButton::OnClick()
{
    m_onClicked();
}

void UIButton::OnExitHover()
{
    Core::INSTANCE->floatTweenManager.Create(m_isHeldDown ? 1.4f : 1.25f)
                  ->to(1)
                  .during(150)
                  .via(tweeny::easing::enumerated::quarticOut)
                  .onStep([this](tweeny::tween<float>& t, float fontSize)
                  {
                      this->m_actualFontSize = this->m_fontSize * fontSize;
                      return false;
                  });

    auto hovered_color = game_assets::text_color;
    auto text_color = game_assets::accent_color;

    Core::INSTANCE->colorTweenManager
                  .Create(text_color.a, text_color.r, text_color.g, text_color.b)
                  ->to(hovered_color.a, hovered_color.r, hovered_color.g, hovered_color.b)
                  .during(50)
                  .onStep([this](auto t, auto a, auto r, auto g, auto b)
                  {
                      this->m_color.a = a;
                      this->m_color.r = r;
                      this->m_color.g = g;
                      this->m_color.b = b;
                      return false;
                  });
}

void UIButton::OnPressed()
{
    Core::INSTANCE->floatTweenManager
        .Create(m_isHovering ? 1.25f : 1)
        ->to(1.4f)
        .during(150)
        .via(tweeny::easing::enumerated::quarticOut)
        .onStep([this](tweeny::tween<float>& t, float fontSize)
        {
            this->m_actualFontSize = this->m_fontSize * fontSize;
            return false;
        });
}

void UIButton::OnEnterHover()
{
    auto core = Core::INSTANCE;
    core->floatTweenManager
        .Create(1)
        ->to(1.25f)
        .during(150)
        .via(tweeny::easing::enumerated::quarticOut)
        .onStep([this](tweeny::tween<float>& t, float fontSize)
        {
            this->m_actualFontSize = this->m_fontSize * fontSize;
            return false;
        });

    auto text_color = game_assets::text_color;
    auto hovered_color = game_assets::accent_color;

    core->colorTweenManager
        .Create(text_color.a, text_color.r, text_color.g, text_color.b)
        ->to(hovered_color.a, hovered_color.r, hovered_color.g, hovered_color.b)
        .during(50)
        .onStep([this](auto t, auto a, auto r, auto g, auto b)
        {
            this->m_color.a = a;
            this->m_color.r = r;
            this->m_color.g = g;
            this->m_color.b = b;
            return false;
        });
}
