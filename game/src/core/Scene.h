#pragma once
#include <vector>

#include "Renderable.h"

class Scene : public Renderable
{
public:
    std::vector<Renderable> renderables;
    void Render(Camera2D& camera) override;
    void Update() override;
};
