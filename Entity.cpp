#include "Entity.hpp"

Entity::Entity(int entityID) : id(entityID), componentBitset(0)
{

}

void Entity::AddComponent(std::shared_ptr<Component> component)
{
    this->components.push_back(component);
    this->componentBitset = this->componentBitset | component->GetComponentType();
    this->typeToIndexMap[component->GetComponentType()] = this->components.size() - 1;
}

bool Entity::HasComponent(std::uint32_t componentType)
{
    return this->componentBitset & componentType;
}

bool Entity::IsEligibleForSystem(std::uint32_t systemBitset)
{
    return this->componentBitset & systemBitset;
}