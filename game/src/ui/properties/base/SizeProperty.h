#pragma once
#include "Property.h"
#include "tweeny/tweeny.h"
#include "ui/Size.h"
#include "utils/Managed.h"

class SizeProperty
{
public:
    SizeProperty(const Size& defaultValue, const std::string& key, const std::function<int()>& max);
    ~SizeProperty();
    
    void Set(const Style& style, bool doTransition);
    float Get() const;
    void Override(float val);

protected:
    virtual Size GetNewValue(const Style& style);
    
    float m_evaled;
    float m_override;
    bool m_overriden;
    
    std::function<int()> m_max;
    std::string m_key;
    Size m_default;

    using tween_type = std::weak_ptr<Managed<tweeny::tween<float>*>>;
    void ManageTween(tween_type tween);
    std::vector<tween_type> m_tweens;
};
