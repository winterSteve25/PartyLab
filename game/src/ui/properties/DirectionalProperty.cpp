#include "DirectionalProperty.h"

#include "core/Core.h"
#include "lua/LuaConstants.h"
#include "ui/Transition.h"

DirectionalProperty::DirectionalProperty(const Size& def, const std::string& key, const std::string& allKey,
                                         const std::function<int()>& max):
    SizeProperty(def, key, max),
    m_allKey(allKey)
{
}

void DirectionalProperty::Set(const Style& style, bool doTransition)
{
    if (m_overriden) return;

    sol::optional<Transition> transition = Style::GetOptionalField<Transition>(
        style.GetOr(UI_PROP_STYLE_TRANSITIONS, sol::optional<sol::table>(sol::nullopt)),
        m_key
    );

    const sol::optional<sol::object> newValueA = style.Get<sol::object>(m_key);
    const sol::optional<sol::object> newAllValue = style.Get<sol::object>(m_allKey);

    Size newValue = newValueA.has_value() ? Size(newValueA.value()) :
                        newAllValue.has_value() ? Size(newAllValue.value()) : m_default;

    if (!doTransition || !transition.has_value())
    {
        m_evaled = static_cast<float>(newValue.Eval(m_max()));
        return;
    }

    auto tween = Core::INSTANCE->floatTweenManager.Create(m_evaled);

    tween->to(static_cast<float>(newValue.Eval(m_max())));
    tween->onStep([this](auto t, auto f)
    {
        this->m_evaled = f;
        return false;
    });

    transition.value().SetupTween(tween);
}
