#pragma once
#include <vector>
#include <memory>

#include "raylib.h"
#include "tweeny/tweeny.h"
#include "utils/Managed.h"

template <typename... Ts>
class SimpleTweenManager
{
public:
    std::weak_ptr<Managed<tweeny::tween<Ts...>*>> Create(Ts... from)
    {
        auto t = std::make_shared<Managed<tweeny::tween<Ts...>*>>(new tweeny::tween<Ts...>(tweeny::from(from...)));
        tweens.push_back(t);
        return t;
    }

    void Tick()
    {
        int dt = static_cast<int>(1000.0f * GetFrameTime());
        for (int i = 0; i < tweens.size(); i++)
        {
            std::shared_ptr<Managed<tweeny::tween<Ts...>*>> tween = tweens[i];

            if (tween->IsKilled())
            {
                delete tween->Value();
                tweens.erase(tweens.begin() + i);
                i--;
                return;
            }

            auto tweenVal = tween->Value();
            tweenVal->step(dt);
            if (tweenVal->progress() < 1) continue;

            // finished
            delete tweenVal;
            tweens.erase(tweens.begin() + i);
            i--;
        }
    }

    void DeleteAll()
    {
        for (int i = 0; i < tweens.size(); i++)
        {
            std::shared_ptr<Managed<tweeny::tween<Ts...>*>> tween = tweens[i];
            delete tween->Value();
            tweens.erase(tweens.begin() + i);
            i--;
        }
    }

private:
    std::vector<std::shared_ptr<Managed<tweeny::tween<Ts...>*>>> tweens;
};
