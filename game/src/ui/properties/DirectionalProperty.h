#pragma once
#include "base/SizeProperty.h"

class DirectionalProperty : public SizeProperty
{
public:
    DirectionalProperty(const Size& def, const std::string& key, const std::string& allKey, const std::function<int()>& max);

protected:
    Size GetNewValue(const Style& style) override;

private:
    std::string m_allKey;
};
