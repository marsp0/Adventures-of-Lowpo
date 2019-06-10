#include "InputComponent.hpp"

InputComponent::InputComponent() : Component(ComponentType::Input)
{
    this->actions.resize(Action::Size);
}

InputComponent::~InputComponent()
{
    
}