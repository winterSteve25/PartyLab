#pragma once

#include "lua/lua_utils.h"
#include "sol/sol.hpp"
#include "tweeny/tween.h"

/**
 * A transition object used to define transition effect in lua ui style
 */
struct Transition
{
public:
    unsigned int duration;
    sol::object ease;
    Transition(const unsigned int duration, const sol::object& ease);

    template <typename T, typename... Ts>
    void SetupTween(tweeny::tween<T, Ts...>* tween)
    {
        tween->during(duration);
        if (ease.is<sol::protected_function>())
        {
            auto e = ease.as<sol::protected_function>();
            tween->via([e](float p, T a, T b)
            {
                return (b - a) * lua_utils::UnwrapResult<float>(e(p), "Errored when running transition function") + a;
            });
        }
    }

private:
    template <class... Ts>
    struct overload : Ts...
    {
        using Ts::operator()...;
    };
};
