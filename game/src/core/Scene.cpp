#include "Scene.h"

void Scene::Render(Camera2D& camera)
{
    for (Renderable* renderable : m_renderables)
    {
        renderable->Render(camera);
    }
}

void Scene::Update()
{
    for (Renderable* renderable : m_renderables)
    {
        renderable->Update();
    }
}

void Scene::Load()
{
}

void Scene::Cleanup()
{
    for (auto ptr : m_renderables)
    {
        delete ptr;
    }

    m_renderables.clear();
}

void Scene::AddRenderable(Renderable* renderable)
{
    this->m_renderables.push_back(renderable);
}
