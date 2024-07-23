#include "Transition.h"

Transition::Transition(const unsigned int duration, const sol::object& ease)
    : duration(duration),
      ease(ease)
{
}
