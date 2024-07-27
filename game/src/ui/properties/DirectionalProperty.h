#pragma once
#include "base/LengthProperty.h"

class DirectionalProperty : public LengthProperty
{
public:
    DirectionalProperty(const Length& def, const std::string& key, const std::string& allKey, bool horizontal);

protected:
    Length GetNewValue(const Style& style) override;

private:
    std::string m_allKey;
};
