#pragma once

class Renderable
{
public:
    virtual ~Renderable() = default;
    virtual void Render() = 0;
    virtual void RenderOverlay() = 0;
    virtual void Update() = 0;
};
