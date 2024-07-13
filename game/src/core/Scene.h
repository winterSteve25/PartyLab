#pragma once
#include <vector>

#include "Renderable.h"

class Scene : public Renderable
{
public:
    void Render(Camera2D& camera) override;
    void Update() override;
    virtual void Load();
    virtual void Cleanup();
protected:
    void AddRenderable(Renderable* renderable);
private:
    std::vector<Renderable*> m_renderables;
};
