#include "UIText.h"

#include "assets/game_assets.h"
#include "lua/LuaConstants.h"
#include "ui/ui_helper.h"
#include "ui/properties/properties.h"

UIText::UIText(const sol::table& table, const std::string& text) :
    UIElement(table),
    m_text(text),
    m_sColor(properties::ColorProp(game_assets::TEXT_COLOR)),
    m_sFontSize(FontSizeProperty(this, &this->m_text))
{
}

UIText::UIText(const sol::table& table):
    UIElement(table),
    m_text(table.get<sol::optional<std::string>>("text").value_or("Empty Text")),
    m_sColor(properties::ColorProp(game_assets::TEXT_COLOR)),
    m_sFontSize(FontSizeProperty(this, &this->m_text))
{
}

void UIText::Render(const lay_context* ctx)
{
    UIElement::Render(ctx);
    Vector2 pos = GetPos(ctx);
    Vector2 size = GetSize(ctx);
    pos.y -= size.y * 0.24f;
    ui_helper::DrawText(m_text.c_str(), m_sFontSize.Get(), pos, m_sColor.Get());
}

void UIText::ApplyStyles(const Style& style, bool doTransition)
{
    m_sColor.Set(style, doTransition);
    m_sFontSize.Set(style, doTransition);
    UIElement::ApplyStyles(style, doTransition);
}
