#include "Scene.h"

void Scene::Render(Camera2D& camera)
{
    for (Renderable renderable : renderables)
    {
        renderable.Render(camera);
    }
}

void Scene::Update()
{
    for (Renderable renderable : renderables)
    {
        renderable.Update();
    }
}
