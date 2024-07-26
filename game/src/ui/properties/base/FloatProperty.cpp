#include "FloatProperty.h"
#include "core/Core.h"
#include "lua/LuaConstants.h"
#include "ui/Transition.h"

FloatProperty::FloatProperty(float defaultValue, const std::string& key):
    TweenManagingProperty(defaultValue, key)
{
}

void FloatProperty::Set(const Style& style, bool doTransition)
{
    const auto newValue = GetNewValue(style);
    if (abs(newValue - m_val) < 0.01)
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
        OnSet();
        return;
    }

    auto tweenPtr = Core::INSTANCE->floatTweenManager.Create(m_val);
    ManageTween(tweenPtr);
    auto tween = tweenPtr.lock()->Value();
    TraceLog(LOG_INFO, "Tween Created");

    tween->to(newValue);
    tween->onStep([this](auto t, auto s)
    {
        this->m_val = s;
        OnSet();

        if (t.progress() < 1)
        {
            return false;
        }
        
        TraceLog(LOG_INFO, "Tween COMPLETED");
        return false;
    });

    transition.value().SetupTween(tween);
}

void FloatProperty::OnSet()
{
}

float FloatProperty::GetNewValue(const Style& style)
{
    return style.GetOr(m_key, m_defaultValue);
}
