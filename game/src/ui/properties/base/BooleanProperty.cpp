#include "BooleanProperty.h"

BooleanProperty::BooleanProperty(bool defaultValue, const std::string& key):
    Property(defaultValue, key)
{
}


void BooleanProperty::Set(const Style& style, bool doTransition)
{
    m_val = style.GetOr(m_key, m_defaultValue);
}
