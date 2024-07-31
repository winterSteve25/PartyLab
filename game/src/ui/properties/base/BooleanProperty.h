#pragma once
#include "Property.h"

class BooleanProperty : public Property<bool>
{
public:
     BooleanProperty(bool defaultValue, const std::string& key);
     void Set(const Style& style, bool doTransition) override;
};
