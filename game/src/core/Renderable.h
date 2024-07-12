#pragma once
#include "raylib.h"

class Renderable
{
public:
    virtual void Render(Camera2D& camera);
    virtual void Update();
};
