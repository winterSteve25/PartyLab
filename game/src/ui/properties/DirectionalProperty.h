#pragma once
#include "base/SizeProperty.h"

class DirectionalProperty : public SizeProperty
{
public:
    DirectionalProperty(const Size& def, const std::string& key, const std::string& allKey, const std::function<int()>& max);
    void Set(const Style& style, bool doTransition) override;
private:
    std::string m_allKey;
};
