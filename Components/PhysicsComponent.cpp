#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent( float mass,
                                    glm::vec3 position,
                                    glm::quat orientation,
                                    glm::mat3 inertiaTensor,
                                    DynamicType dynamicType) : \
                                    position(position),
                                    orientation(orientation),
                                    dynamicType(dynamicType),
                                    Component(ComponentType::Physics),
                                    acceleration(0.f),
                                    velocity(0.f),
                                    forceAccumulator(0.f),
                                    angularAcc(0.f),
                                    angularVel(0.f)
{
    // invert the mass and inertia tensor here
}

PhysicsComponent::~PhysicsComponent()
{

}
