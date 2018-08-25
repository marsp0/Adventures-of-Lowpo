#pragma once

#include "GameObject.hpp"
#include "Camera.hpp"

enum PlayerActions
{
    MOVE_FOREWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP
};

class Player : public GameObject
{
    public:
        Player( Transform                   transform, 
                std::shared_ptr<Texture>    texture, 
                std::shared_ptr<Mesh>       mesh,
                float cameraWidth,
                float cameraHeight);

        void HandleInput(GLFWwindow* window);
        void Update(float deltaTime);

    private:

        Camera  camera;

        // Input
        bool    actions[5];
        double  lastPositionX;
        double  lastPositionY;
};