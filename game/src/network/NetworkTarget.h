#pragma once
#include <cstdint>

enum NetworkTarget : uint8_t
{
    EVERYONE = 0,
    EVERYONE_ELSE,
    HOST,
    PLAYER,
};
