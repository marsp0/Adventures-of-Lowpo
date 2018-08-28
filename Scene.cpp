#include "Scene.hpp"

Scene::Scene(float width, float height)
{
    
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
    this->gameObjects.push_back(gameObject);
}

void Scene::DeleteGameObject(std::shared_ptr<GameObject> gameObject)
{
    
}

void Scene::Update(float deltaTime)
{
    // NOTE : move the object updating here.
}

void Scene::AddCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}