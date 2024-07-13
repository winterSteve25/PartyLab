#pragma once
#include "raylib.h"

class Renderable
{
public:
    virtual ~Renderable() = default;
    virtual void Render(Camera2D& camera) = 0;
    virtual void Update() = 0;
};
