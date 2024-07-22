#include "BackgroundColorProperty.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"

BackgroundColorProperty::BackgroundColorProperty(UIElement* parent)
    : ColorProperty(parent)
{
}

void BackgroundColorProperty::Set(const Style& style, bool doTransition)
{
    sol::optional<Transition> transition = Style::GetOptionalField<Transition>(
        style.transitions, UI_PROP_STYLE_BACKGROUND_COLOR);

    if (!doTransition || !transition.has_value())
    {
        m_val = style.backgroundColor;
        return;
    }

    if (transition.has_value())
    {
        const auto [r, g, b, a] = style.backgroundColor.val;
        auto tween = Core::INSTANCE->colorTweenManager.Create(m_val.r, m_val.g, m_val.b, m_val.a);
        tween->to(r, g, b, a);
        tween->onStep([this](auto t, auto r, auto g, auto b, auto a)
        {
            this->m_val.r = r;
            this->m_val.g = g;
            this->m_val.b = b;
            this->m_val.a = a;
            return false;
        });

        transition.value().SetupTween(tween);
    }
}
