#include "Scene.hpp"

Scene::Scene(float width, float height) :
    camera(width, height)
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

}