#include "PhysicsComponent.hpp"
#include <glm/glm.hpp>

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
                                    torqueAccumulator(0.f),
                                    angularAcc(0.f),
                                    angularVel(0.f)
{
      assert(mass != 0.f);
      this->inverseMass = 1/mass;
      this->invInertiaTensor = glm::inverse(inertiaTensor);
}

PhysicsComponent::~PhysicsComponent()
{

}
