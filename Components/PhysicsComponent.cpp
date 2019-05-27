#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent( float mass,
                                    glm::vec3 position,
                                    glm::quat orientation,
                                    glm::mat3 inertiaTensor,
                                    DynamicType dynamicType) : \
                                    position(position),
                                    orientation(orientation),
                                    dynamicType(dynamicType),
                                    Component(ComponentType::Physics)
{
    // invert the mass and inertia tensor here
}

PhysicsComponent::~PhysicsComponent()
{

}
