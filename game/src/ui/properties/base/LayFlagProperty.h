#pragma once
#include "Property.h"

class LayFlagProperty : public Property<uint32_t>
{
public:
    LayFlagProperty(const unsigned& defaultValue, const std::string& key);
    void Set(const Style& style, bool doTransition) override;
};
