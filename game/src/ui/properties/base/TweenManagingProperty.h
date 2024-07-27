#pragma once
#include "Property.h"
#include "tweeny/tween.h"
#include "utils/Managed.h"

/**
 * A property with helper methods to manage tweens to automatically cancel tweens when this property is destroyed (when UIComponent with this property is destroyed)
 * @tparam T 
 * @tparam TweenTs Type used by the tween
 */
template <typename T, typename... TweenTs>
class TweenManagingProperty : public Property<T>
{
public:
    TweenManagingProperty(const T& defaultValue, const std::string& key)
        : Property<T>(defaultValue, key)
    {
    }

    ~TweenManagingProperty() override
    {
        for (tween_type tween : m_tweens)
        {
            if (auto t = tween.lock())
            {
                t->Kill();
            }
        }

        m_tweens.clear();
    }

private:
    using tween_type = std::weak_ptr<Managed<tweeny::tween<TweenTs...>*>>;

protected:
    void ManageTween(tween_type tween)
    {
        m_tweens.push_back(tween);
    }
    
    std::vector<tween_type> m_tweens;
};
