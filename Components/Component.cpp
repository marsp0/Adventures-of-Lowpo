#include "Component.hpp"

Component::Component(std::uint32_t type) : type(type)
{

}

Component::~Component()
{
	
}

std::uint32_t Component::GetComponentType()
{
    return this->type;
}