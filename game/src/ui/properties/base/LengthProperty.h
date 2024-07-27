#pragma once
#include "Property.h"
#include "tweeny/tweeny.h"
#include "ui/Length.h"
#include "utils/Managed.h"

class LengthProperty
{
public:
    LengthProperty(const Length& defaultValue, const std::string& key, bool horizontal);
    ~LengthProperty();
    
    void Set(const Style& style, bool doTransition);
    float Get() const;
    void Override(float val);

protected:
    virtual Length GetNewValue(const Style& style);
    
    float m_evaled;
    float m_override;
    bool m_overriden;
    
    bool m_isHorizontal;
    std::string m_key;
    Length m_default;

    using tween_type = std::weak_ptr<Managed<tweeny::tween<float>*>>;
    void ManageTween(tween_type tween);
    std::vector<tween_type> m_tweens;
};
