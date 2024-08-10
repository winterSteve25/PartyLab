#pragma once
#include "base/FloatProperty.h"

class FontSizeProperty : public FloatProperty
{
public:
    FontSizeProperty(UIElement* parent, const std::string* text);
    void Remeasure();

protected:
    void OnSet() override;
    float GetNewValue(const Style& style) override;

private:
    const std::string* m_parentText;
    UIElement* m_parent;
};
