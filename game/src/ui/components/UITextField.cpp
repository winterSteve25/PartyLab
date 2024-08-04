#include "UITextField.h"

#include <format>

#include "assets/game_assets.h"
#include "lua/lua_utils.h"
#include "ui/ui_helper.h"
#include "ui/properties/properties.h"

UITextField::UITextField(const sol::table& table):
    UIElement(table),
    m_fontSize(FontSizeProperty(this, &this->m_placeholderText)),
    m_onSubmit(table.get<sol::optional<sol::protected_function>>("onSubmit")),
    m_placeholderText(table.get<std::optional<std::string>>("placeholder").value_or("Enter something here")),
    m_focused(false),
    m_cursorIdx(0)
{
}

void UITextField::Render(const lay_context* ctx)
{
    UIElement::Render(ctx);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_focused = ui_helper::Within(GetMousePosition(), GetPos(ctx), GetSize(ctx));
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        Submit();
    }

    Vector2 pos = GetPos(ctx);
    Vector2 size = GetSize(ctx);

    BeginScissorMode(pos.x, pos.y, size.x, size.y);

    if (m_focused)
    {
        if (IsKeyDown(KEY_BACKSPACE))
        {
            m_backspaceTime += GetFrameTime();
        }
        else
        {
            m_backspaceTime = 0;
        }

        if (IsKeyDown(KEY_DELETE))
        {
            m_deleteTime += GetFrameTime();
        }
        else
        {
            m_deleteTime = 0;
        }

        Vector2 textSize = ui_helper::MeasureText(m_text.substr(0, m_cursorIdx).c_str(), m_fontSize.Get());

        if (m_text.empty())
        {
            textSize.x = 0;
        }

        DrawRectangle(static_cast<int>(pos.x + textSize.x), static_cast<int>(pos.y), 2, static_cast<int>(size.y),
                      game_assets::BACKGROUND_COLOR);

        if (const char pressed = GetCharPressed(); pressed != 0)
        {
            m_text.insert(m_cursorIdx, 1, pressed);
            m_cursorIdx++;
        }

        if (!m_text.empty())
        {
            if (m_cursorIdx > 0 && (IsKeyPressed(KEY_BACKSPACE) || m_backspaceTime > 0.6))
            {
                m_text.erase(m_cursorIdx - 1, 1);
                m_cursorIdx--;
            }
            else if (m_cursorIdx < m_text.size() + 1 && (IsKeyPressed(KEY_DELETE) || m_deleteTime > 0.6))
            {
                m_text.erase(m_cursorIdx, 1);
            }
        }

        if (IsKeyPressed(KEY_LEFT) && m_cursorIdx > 0)
        {
            m_cursorIdx--;
        }

        if (IsKeyPressed(KEY_RIGHT) && m_cursorIdx < m_text.size())
        {
            m_cursorIdx++;
        }
    }

    pos.y -= size.y * 0.24f;

    if (m_text.empty() && !m_focused)
    {
        Color c = game_assets::BACKGROUND_COLOR;
        c.a = 100;
        ui_helper::DrawText(m_placeholderText.c_str(), m_fontSize.Get(), pos, c);
    }

    if (!m_text.empty())
    {
        ui_helper::DrawText(m_text.c_str(), m_fontSize.Get(), pos, game_assets::BACKGROUND_COLOR);
    }

    EndScissorMode();
}

void UITextField::ApplyStyles(const Style& style, bool doTransition)
{
    UIElement::ApplyStyles(style, doTransition);
    m_fontSize.Set(style, doTransition);
}

sol::table UITextField::CreateLuaObject(lua_State* L)
{
    sol::table table = UIElement::CreateLuaObject(L);
    table["submit"] = [this]() { Submit(); };
    return table;
}

void UITextField::Submit()
{
    if (!m_onSubmit.has_value()) return;
    lua_utils::UnwrapResult(m_onSubmit.value()(m_text), "Failed to run on submit callback");
    m_text.clear();
    m_cursorIdx = 0;
}
