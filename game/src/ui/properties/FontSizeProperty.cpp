#include "FontSizeProperty.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"
#include "ui/ui_helper.h"
#include "ui/components/UIElement.h"

FontSizeProperty::FontSizeProperty(UIElement* parent, const std::string* text):
    Property(parent),
    m_parentText(text)
{
}

void FontSizeProperty::Set(const Style& style, bool doTransition)
{
    sol::optional<Transition> transition = Style::GetOptionalField<Transition>(
        style.transitions, UI_PROP_STYLE_FONTSIZE);

    if (!doTransition || !transition.has_value())
    {
        m_val = style.fontSize;
        Vector2 size = ui_helper::MeasureText(this->m_parentText->c_str(), m_val);
        lay_set_size_xy(
            this->m_parent->layCtx,
            this->m_parent->layId,
            static_cast<lay_scalar>(size.x),
            static_cast<lay_scalar>(size.y)
        );
        return;
    }

    const auto fs = style.fontSize.val;
    auto tween = Core::INSTANCE->floatTweenManager.Create(m_val);
    tween->to(fs);
    tween->onStep([this](auto t, auto s)
    {
        this->m_val = s;
        Vector2 size = ui_helper::MeasureText(this->m_parentText->c_str(), s);
        lay_set_size_xy(
            this->m_parent->layCtx,
            this->m_parent->layId,
            static_cast<lay_scalar>(size.x),
            static_cast<lay_scalar>(size.y)
        );
        return false;
    });

    transition.value().SetupTween(tween);
}
