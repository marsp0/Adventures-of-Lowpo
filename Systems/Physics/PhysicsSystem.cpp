#include "PhysicsSystem.hpp"

#include "../../Components/InputComponent.hpp"

PhysicsSystem::PhysicsSystem(float gridLength, float cellHalfWidth) : grid(gridLength, cellHalfWidth)
{
    this->primaryBitset = ComponentType::Physics | ComponentType::Transform;
}

PhysicsSystem::~PhysicsSystem()
{

}

void PhysicsSystem::Insert(std::vector<std::shared_ptr<Collider>>& colliders)
{
    for (int i = 0; i < colliders.size(); i++)
    {
        this->grid.Insert(colliders[i]);
    }
}

void PhysicsSystem::Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities, std::vector<Message>& messages, std::vector<Message>& globalQueue)
{
    // build entity -> messages map
    std::unordered_map<int, std::vector<Message>> idToMessage;
    for (int i = 0; i < messages.size(); i++)
    {
        idToMessage[messages[i].senderID].push_back(messages[i]);
    }

    std::unordered_map<int, int> idToIndexMap;
    // Integration step
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->primaryBitset))
        {
            // handle messages for the current entity

            idToIndexMap[entities[i]->id] = i;
            PhysicsComponent component = entities[i]->GetComponent<PhysicsComponent>(ComponentType::Physics);
            // acceleration update
            component.acceleration += component.forceAccumulator * component.inverseMass;
            component.angularAcc += component.invInertiaTensor * component.torqueAccumulator;
            // velocity update
            component.velocity += component.acceleration * deltaTime;
            component.angularVel += component.angularAcc * deltaTime;
            // position update
            component.position += component.velocity * deltaTime;
            component.orientation.x += 0.5f * component.orientation.x * component.angularVel.x * deltaTime;
            component.orientation.y += 0.5f * component.orientation.y * component.angularVel.y * deltaTime;
            component.orientation.z += 0.5f * component.orientation.z * component.angularVel.z * deltaTime;

            glm::vec3 velocityChange = component.velocity * deltaTime;
            for (int j = 0; j < component.colliders.size(); j++)
            {
                component.colliders[j]->Update(velocityChange);
                int newRow = this->grid.GetInsertRow(component.colliders[j]->center);
                int newCol = this->grid.GetInsertCol(component.colliders[j]->center);
                int oldRow = component.colliders[j]->row;
                int oldCol = component.colliders[j]->col;
                if (oldRow != newRow || oldCol || newCol)
                {
                    this->grid.Remove(component.colliders[j]);
                    this->grid.Insert(component.colliders[j]);
                }
            }
        }
    } 
    // 2. Check for collision
    std::vector<std::shared_ptr<Collision>> collisions = this->grid.CheckCollisions();
    // 3. Resolve Collisions
    this->Solve(entities, collisions, idToIndexMap);
    // 4. Resolve Interpenetration
    // TO DO
}

void PhysicsSystem::Solve(std::vector<std::shared_ptr<Entity>>& entities, std::vector<std::shared_ptr<Collision>>& collisions, std::unordered_map<int, int>& idToIndexMap)
{
    float ELASTICITY = 0.4f;
    for (int i = 0; i < collisions.size(); i++)
    {
        std::shared_ptr<Collision> collision = collisions[i];
        int firstEntityIndex = idToIndexMap[collision->first];
        int secondEntityIndex = idToIndexMap[collision->second];

        PhysicsComponent first = entities[firstEntityIndex]->GetComponent<PhysicsComponent>(ComponentType::Physics);
        PhysicsComponent second = entities[secondEntityIndex]->GetComponent<PhysicsComponent>(ComponentType::Physics);

        std::shared_ptr<Collider> firstCollider = collision->firstCollider;
        std::shared_ptr<Collider> secondCollider = collision->secondCollider;

        for (int j = 0; j < collision->contacts.size(); j++)
        {
            Contact contact = collision->contacts[j];
            // pre - prefix used to indicate pre collision variable value
            // post - prefix used to indicate post collision variable value
            // find impulse
            glm::vec3 normal = contact.contactNormal;
            glm::vec3 rA = normal - firstCollider->center;
            glm::vec3 rB = normal - secondCollider->center;
            glm::vec3 vA = first.velocity;
            glm::vec3 wA = first.angularVel;
            glm::vec3 vB = second.velocity;
            glm::vec3 wB = second.angularVel;
            float invMassA = first.inverseMass;
            float invMassB = second.inverseMass;
            glm::mat3 invInertiaTensorA = first.invInertiaTensor;
            glm::mat3 invInertiaTensorB = second.invInertiaTensor;

            glm::vec3 preRelativeVelocity = vA + glm::cross(wA, rA) - vB - glm::cross(wB, rB);
            
            float denominator1 = glm::dot(normal, normal) * invMassA + invMassB;
            glm::vec3 denominator3 = glm::cross(invInertiaTensorA * glm::cross(rA, normal), rA);
            glm::vec3 denominator4 = glm::cross(invInertiaTensorB * glm::cross(rB, normal), rB);
            float denominator2 = glm::dot(denominator3 + denominator4, normal);

            float impulse = glm::dot((-1.0f - ELASTICITY) * preRelativeVelocity, normal) / (denominator1 + denominator2);
            
            // update linear velocity of objects based on impulse
            glm::vec3 vA2 = vA + impulse * normal * invMassA;
            glm::vec3 vB2 = vB - impulse * normal * invMassB;

            // update angular velocity of objects based on impulse.
            glm::vec3 wA2 = wA + invInertiaTensorA * glm::cross(rA, impulse * normal);
            glm::vec3 wB2 = wB - invInertiaTensorB * glm::cross(rB, impulse * normal);

            first.velocity = vA2;
            first.angularVel = wA2;
            
            second.velocity = vB2;
            second.angularVel = wB2;
        }
    }
}

void PhysicsSystem::HandleEvent(Message& event, PhysicsComponent& component)
{
    // if (event.type == EventType::Move)
    // {
    // }

}