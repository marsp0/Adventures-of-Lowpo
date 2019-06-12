#include "InputSystem.hpp"
#include "../../Entity.hpp"
#include "../../Components/InputComponent.hpp"

InputSystem::InputSystem()
{
    this->primaryBitset = ComponentType::Input;
}

InputSystem::~InputSystem()
{

}

void InputSystem::Update(GLFWwindow* window, std::vector<std::shared_ptr<Entity>>& entities, std::vector<Event>& events, std::vector<Event>& globalQueue)
{
    // Note : we probably want to add animation triggers here.
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->primaryBitset))
        {
            
            // KEYBOARD
            std::vector<bool> actionList(4);
            bool generateMoveMessage = false;
            InputComponent component = entities[i]->GetComponent<InputComponent>(ComponentType::Input);
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                actionList[Action::MoveForward] = true;
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                actionList[Action::MoveBackward] = true;
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                actionList[Action::MoveLeft] = true;
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                actionList[Action::MoveRight] = true;
            }

            // Mouse
            
            // generate message
            for (int j = 0; j < actionList.size(); j++)
            {
                if (actionList[j])
                    generateMoveMessage = true;
            }

            if (generateMoveMessage)
            {
                Event event(entities[i]->id, 0, EventType::Move);
                std::shared_ptr<MoveData> moveData = std::make_shared<MoveData>(actionList[Action::MoveForward],
                                                                                 actionList[Action::MoveBackward],
                                                                                 actionList[Action::MoveLeft],
                                                                                 actionList[Action::MoveRight]);
                event.data = moveData;
            }
        }
    }
}