#include "InputSystem.hpp"
#include "InputComponent.hpp"
#include "Entity.hpp"

InputSystem::InputSystem()
{
    this->primaryBitset = ComponentType::Input;
}

InputSystem::~InputSystem()
{

}

void InputSystem::Update(GLFWwindow* window, std::vector<std::shared_ptr<Entity>>& entities)
{
    // Note : we probably want to add animation triggers here.
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->primaryBitset))
        {
            InputComponent component = entities[i]->GetComponent<InputComponent>(ComponentType::Input);
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                component.actions[Action::MoveForward] = true;
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                component.actions[Action::MoveBackward] = true;
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                component.actions[Action::MoveLeft] = true;
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                component.actions[Action::MoveRight] = true;
            }
        }
    }
}