#include "SizeProperty.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"
#include "ui/Transition.h"

SizeProperty::SizeProperty(const Size& defaultValue, const std::string& key, const std::function<int()>& max):
    m_max(max),
    m_key(key),
    m_default(defaultValue),
    m_overriden(false),
    m_override(0)
{
    m_evaled = static_cast<float>(defaultValue.Eval(max()));
}

SizeProperty::~SizeProperty()
{
    for (tween_type tween : m_tweens)
    {
        if (auto t = tween.lock())
        {
            t->Kill();
        }
    }

    m_tweens.clear();
}

void SizeProperty::ManageTween(tween_type tween)
{
    m_tweens.push_back(tween);
}


void SizeProperty::Set(const Style& style, bool doTransition)
{
    if (m_overriden) return;

    sol::optional<Transition> transition = Style::GetOptionalField<Transition>(
        style.GetOr(UI_PROP_STYLE_TRANSITIONS, sol::optional<sol::table>(sol::nullopt)),
        m_key
    );

    Size newValue = GetNewValue(style);

    if (!doTransition || !transition.has_value())
    {
        m_evaled = static_cast<float>(newValue.Eval(m_max()));
        return;
    }

    auto tweenPtr = Core::INSTANCE->floatTweenManager.Create(m_evaled);
    ManageTween(tweenPtr);
    const auto tween = tweenPtr.lock()->Value();

    tween->to(static_cast<float>(newValue.Eval(m_max())));
    tween->onStep([this](auto t, auto f)
    {
        this->m_evaled = f;
        return false;
    });

    transition.value().SetupTween(tween);
}

float SizeProperty::Get() const
{
    if (m_overriden) return m_override;
    return m_evaled;
}

void SizeProperty::Override(float val)
{
    m_overriden = true;
    m_override = val;
}

Size SizeProperty::GetNewValue(const Style& style)
{
    const sol::optional<sol::object> newValueA = style.Get<sol::object>(m_key);
    return newValueA.has_value() ? Size(newValueA.value()) : m_default;
}
