#include "ColorProperty.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"

ColorProperty::ColorProperty(UIElement* parent)
    : Property<Color>(parent)
{
}

void ColorProperty::Set(const Style& style, bool doTransition)
{
    sol::optional<Transition> transition = Style::GetOptionalField<Transition>(style.transitions, UI_PROP_STYLE_COLOR);

    if (!doTransition || !transition.has_value())
    {
        m_val = style.color;
        return;
    }

    if (transition.has_value())
    {
        const auto [r, g, b, a] = style.color.val;
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
