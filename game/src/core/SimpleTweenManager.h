#pragma once
#include <vector>

#include "tweeny/tweeny.h"

template <typename... Ts>
class SimpleTweenManager
{
public:
    tweeny::tween<Ts...>* Create(Ts... from)
    {
        tweeny::tween<Ts...>* t = new tweeny::tween<Ts...>(tweeny::from(from...));
        tweens.push_back(t);
        return t;
    }

    void Tick()
    {
        int dt = static_cast<int>(1000.0f * GetFrameTime());
        for (int i = 0; i < tweens.size(); i++)
        {
            tweeny::tween<Ts...>* tween = tweens[i];
            tween->step(dt);
            if (tween->progress() < 1) continue;

            // finished
            delete tween;
            tweens.erase(tweens.begin() + i);
            i--;
        }
    }

    void DeleteAll()
    {
        for (int i = 0; i < tweens.size(); i++)
        {
            tweeny::tween<Ts...>* tween = tweens[i];
            delete tween;
            tweens.erase(tweens.begin() + i);
            i--;
        }
    }
private:
    std::vector<tweeny::tween<Ts...>*> tweens;
};
