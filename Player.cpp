#include "Player.hpp"
#include "glm/gtx/rotate_vector.hpp"

Player::Player( Transform                   transform,
                std::shared_ptr<Texture>    texture,
                std::shared_ptr<Mesh>       mesh,
                PhysicsComponent physicsComponent,
                float cameraWidth,
                float cameraHeight) :
                GameObject(transform, texture, mesh,physicsComponent),
                camera(cameraWidth, cameraHeight)
{

}

void Player::HandleInput(GLFWwindow* window)
{
    // KEYBOARD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->actions[PlayerActions::MOVE_FOREWARD] = true;
        std::cout << "Annie are you ok? " << glfwGetKey(window, GLFW_KEY_W) << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        this->actions[PlayerActions::MOVE_BACKWARD] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->actions[PlayerActions::MOVE_LEFT] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->actions[PlayerActions::MOVE_RIGHT] = true;
    }
    
    // // MOUSE
    // double xpos, ypos;
    // glfwGetCursorPos(window, &xpos, &ypos);
    // float xoffset = xpos - this->lastPositionX;
    // float yoffset = this->lastPositionY - ypos;

    // this->lastPositionX = xpos;
    // this->lastPositionY = ypos;

    // this->camera.HandleMouseInput(deltaTime,xoffset,yoffset);
}

void Player::Update(float deltaTime)
{
    if (this->actions[PlayerActions::MOVE_FOREWARD])
    {
        // Proj of cam dir on xz plane
        std::cout << "it is true" << std::endl;
        this->SetVelocity(this->camera.direction - glm::dot(this->camera.direction,this->camera.worldUp)*this->camera.worldUp);
        this->actions[PlayerActions::MOVE_FOREWARD] = false;
        std::cout << this->physicsComponent.position.x << std::endl;
        std::cout << this->physicsComponent.position.y << std::endl;
        std::cout << this->physicsComponent.position.z << std::endl;
        std::cout << std::endl;
    }
    if (this->actions[PlayerActions::MOVE_BACKWARD])
    {
        // negate proj cam dir on zn plane
        this->actions[PlayerActions::MOVE_BACKWARD] = false;
        this->SetVelocity(-(this->camera.direction - glm::dot(this->camera.direction,this->camera.worldUp)*this->camera.worldUp));
    }
    if (this->actions[PlayerActions::MOVE_LEFT])
    {
        // rotate velocity by 90 degrees
        this->actions[PlayerActions::MOVE_LEFT] = false;
        this->SetVelocity(glm::rotate(this->GetVelocity(),glm::radians(90.f),this->camera.worldUp));
    }
    if (this->actions[PlayerActions::MOVE_RIGHT])
    {
        // rotate velocity by -90 degrees
        this->actions[PlayerActions::MOVE_RIGHT] = false;
        this->SetVelocity(glm::rotate(this->GetVelocity(),glm::radians(-90.f),this->camera.worldUp));
    }
}