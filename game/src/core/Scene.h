#pragma once
#include <vector>

#include "Renderable.h"

class Scene : public Renderable
{
public:
    Scene();
    void Render() override;
    void RenderOverlay() override;
    void Update() override;
    virtual void Load();
    virtual void Cleanup();
protected:
    void AddRenderable(Renderable* renderable);
private:
    std::vector<Renderable*> m_renderables;
};
