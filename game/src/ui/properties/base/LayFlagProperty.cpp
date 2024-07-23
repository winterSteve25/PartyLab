#include "LayFlagProperty.h"

LayFlagProperty::LayFlagProperty(const unsigned& defaultValue, const std::string& key)
    : Property(defaultValue, key)
{
}

void LayFlagProperty::Set(const Style& style, bool doTransition)
{
    m_val = style.GetOr(m_key, m_defaultValue);
}
