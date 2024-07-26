#include "DirectionalProperty.h"

#include "core/Core.h"
#include "ui/Transition.h"

DirectionalProperty::DirectionalProperty(const Size& def, const std::string& key, const std::string& allKey,
                                         const std::function<int()>& max):
    SizeProperty(def, key, max),
    m_allKey(allKey)
{
}

Size DirectionalProperty::GetNewValue(const Style& style)
{
    const sol::optional<sol::object> newValueA = style.Get<sol::object>(m_key);
    const sol::optional<sol::object> newAllValue = style.Get<sol::object>(m_allKey);

    return newValueA.has_value()
                        ? Size(newValueA.value())
                        : newAllValue.has_value()
                        ? Size(newAllValue.value())
                        : m_default;
}
