#include "Entity.hpp"

Entity::Entity(int entityID) : id(entityID), componentBitset(0), typeToIndexMap()
{

}

void Entity::AddComponent(std::unique_ptr<Component> component)
{
    this->componentBitset = this->componentBitset | component->GetComponentType();
    this->typeToIndexMap[component->GetComponentType()] = this->components.size();
    this->components.push_back(std::move(component));
}

bool Entity::HasComponent(std::uint32_t componentType)
{
    return this->componentBitset & componentType;
}

bool Entity::IsEligibleForSystem(std::uint32_t systemBitset)
{
    return this->componentBitset & systemBitset;
}