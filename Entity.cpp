#include "Entity.hpp"

Entity::Entity()
{

}

void Entity::AddComponent(std::shared_ptr<Component> component)
{
    this->components.push_back(component);
    this->componentBitset = this->componentBitset | component->GetComponentType();
    this->typeToIndexMap[component->GetComponentType()] = this->components.size() - 1;
}

