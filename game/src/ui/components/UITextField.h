#pragma once
#include "UIElement.h"
#include "ui/properties/FontSizeProperty.h"

class UITextField : public UIElement
{
public:
    UITextField(const sol::table& table);
    ~UITextField() override = default;
    void Render(const lay_context* ctx) override;
    void ApplyStyles(const Style& style, bool doTransition) override;
    sol::table CreateLuaObject(lua_State* L) override;
protected:
    FontSizeProperty m_fontSize;
private:
    void Submit();
    sol::optional<sol::protected_function> m_onSubmit;
    std::string m_placeholderText;
    std::string m_text;
    bool m_focused;
    uint32_t m_cursorIdx;
    
    float m_backspaceTime;
    float m_deleteTime;
    float m_idleTime;
};
