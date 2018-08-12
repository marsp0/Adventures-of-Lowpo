#pragma once

#include <memory>
#include <vector>
#include "Camera.hpp"
#include "GameObject.hpp"

class Scene
{
    public:

        Scene(float width, float height);

        void AddGameObject(std::shared_ptr<GameObject> gameObject);
        void DeleteGameObject(std::shared_ptr<GameObject> gameObject);

        void Update(float deltaTime);

        Camera                                      camera;
        std::vector<std::shared_ptr<GameObject>>    gameObjects;

    private:

        
        
};