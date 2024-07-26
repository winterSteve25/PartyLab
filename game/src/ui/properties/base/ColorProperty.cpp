#include "ColorProperty.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"
#include "ui/Transition.h"

ColorProperty::ColorProperty(const Color& defaultValue, const std::string& key):
    TweenManagingProperty(defaultValue, key)
{
}

void ColorProperty::Set(const Style& style, bool doTransition)
{
    const auto newValue = style.GetOr(m_key, m_defaultValue);
    if (newValue.r == m_val.r && newValue.g == m_val.g && newValue.b == m_val.b && newValue.a == m_val.a)
    {
        return;
    }

    sol::optional<Transition> transition = Style::GetOptionalField<Transition>(
        style.GetOr(UI_PROP_STYLE_TRANSITIONS, sol::optional<sol::table>(sol::nullopt)),
        m_key
    );

    if (!doTransition || !transition.has_value())
    {
        m_val = newValue;
        return;
    }

    auto tweenptr = Core::INSTANCE->colorTweenManager.Create(m_val.r, m_val.g, m_val.b, m_val.a);
    ManageTween(tweenptr);
    const auto tween = tweenptr.lock()->Value();

    tween->to(newValue.r, newValue.g, newValue.b, newValue.a);
    tween->onStep([this](auto t, auto r, auto g, auto b, auto a)
    {
        this->m_val = {r, g, b, a};
        return false;
    });

    transition.value().SetupTween(tween);
}
