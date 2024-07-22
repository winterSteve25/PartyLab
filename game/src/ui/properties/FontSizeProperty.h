#pragma once
#include "Property.h"

class FontSizeProperty : public Property<float>
{
public:
    FontSizeProperty(UIElement* parent, const std::string* text);
    void Set(const Style& style, bool doTransition) override;
private:
    const std::string* m_parentText;
};
