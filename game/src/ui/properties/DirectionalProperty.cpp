#include "DirectionalProperty.h"

#include "core/Core.h"
#include "ui/Transition.h"

DirectionalProperty::DirectionalProperty(const Length& def, const std::string& key, const std::string& allKey,
                                         bool isHorizontal):
    LengthProperty(def, key, isHorizontal),
    m_allKey(allKey)
{
}

Length DirectionalProperty::GetNewValue(const Style& style)
{
    const sol::optional<sol::object> newValueA = style.Get<sol::object>(m_key);
    const sol::optional<sol::object> newAllValue = style.Get<sol::object>(m_allKey);

    return newValueA.has_value()
                        ? Length(newValueA.value())
                        : newAllValue.has_value()
                        ? Length(newAllValue.value())
                        : m_default;
}
