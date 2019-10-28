#include <iostream>
#include <vector>
#include <cmath>
#include "catch.hpp"
#include "../src/Systems/Physics/PhysicsSystem.hpp"
#include "../src/Systems/Physics/Plane.hpp"
#include "../src/Systems/Physics/AABB.hpp"
#include "../src/Components/PhysicsComponent.hpp"
#include "../src/Components/TransformComponent.hpp"

TEST_CASE("PhysicsSystem Test")
{
	// System setup
	PhysicsSystem physicsSystem(200.f, 10.f);

	// Entity setup
	// Plane object
	glm::quat orientation(1.0, 0.f, 0.f, 0.f);
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(1.8f, 4.f, 0.f));
	points1.push_back(glm::vec3(1.8f, 4.f, 4.f));
	points1.push_back(glm::vec3(1.8f, 0.f, 4.f));
	points1.push_back(glm::vec3(1.8f, 0.f, 0.f));
	glm::vec3 center1 = glm::vec3(1.8f, 2.f, 2.f);
	std::shared_ptr<Plane> plane1 = std::make_shared<Plane>(Plane(1, center1, ColliderType::PLANE, DynamicType::Static, points1));
	float mass = 1000000.f;
	float coeff = (1.f/12.f)*mass;
	glm::mat3 inertiaTensor = glm::mat3(
		coeff*4, 0.f, 0.f,
		0.f, coeff*4, 0.f,
		0.f, 0.f, coeff*8);
	std::shared_ptr<PhysicsComponent> component1 = std::make_shared<PhysicsComponent>(PhysicsComponent(mass, center1, orientation, inertiaTensor, DynamicType::Static));
	std::shared_ptr<TransformComponent> transformComponent1 = std::make_shared<TransformComponent>(TransformComponent(center1, orientation));
	component1->colliders.push_back(plane1);
	std::shared_ptr<Entity> entity1 = std::make_shared<Entity>(Entity(1));
	entity1->AddComponent(component1);
	entity1->AddComponent(transformComponent1);

	// aabb object
	glm::vec3 box_center1 = glm::vec3(4.f, 2.f, 2.f);
	glm::vec3 axisRadii1 = glm::vec3(2.f,2.f,2.f);
	float xSquared = std::pow(axisRadii1.x * 2.f, 2.f);
	float ySquared = std::pow(axisRadii1.y * 2.f, 2.f);
	float zSquared = std::pow(axisRadii1.z * 2.f, 2.f);
	mass = 4.f;
	coeff = (1.f/12.f)*mass;

	glm::mat3 inertiaTensorBox = glm::mat3(
		coeff*(ySquared + zSquared), 0.f, 0.f,
		0.f, coeff*(xSquared + zSquared), 0.f,
		0.f, 0.f, coeff*(xSquared + ySquared));
	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, box_center1, ColliderType::BOX, DynamicType::Dynamic, axisRadii1));
	std::shared_ptr<PhysicsComponent> component2 = std::make_shared<PhysicsComponent>(PhysicsComponent(mass, box_center1, orientation, inertiaTensorBox, DynamicType::WithPhysics));
	std::shared_ptr<TransformComponent> transformComponent2 = std::make_shared<TransformComponent>(TransformComponent(box_center1, orientation));
	component2->velocity = glm::vec3(-19.f, 0.f, 0.f);
	component2->colliders.push_back(box1);
	std::shared_ptr<Entity> entity2 = std::make_shared<Entity>(Entity(3));
	entity2->AddComponent(component2);
	entity2->AddComponent(transformComponent2);
	std::vector<std::shared_ptr<Collider>> colliders{plane1, box1};
	physicsSystem.Insert(colliders);

	std::vector<std::shared_ptr<Entity>> entities{entity1, entity2};
	std::vector<Message> messages;
	std::vector<Message> globalQueue;
	physicsSystem.Update(0.0166f, entities, messages, globalQueue);
	REQUIRE(entities.size() == 2);
	PhysicsComponent* c1 = entities[1]->GetComponent<PhysicsComponent>(ComponentType::Physics);
	std::cout << c1->position.x << std::endl;
	std::cout << c1->position.y << std::endl;
	std::cout << c1->position.z << std::endl;

	// PhysicsComponent* c2 = entities[0]->GetComponent<PhysicsComponent>(ComponentType::Physics);
	// std::cout << c2->position.x << std::endl;
	// std::cout << c2->position.y << std::endl;
	// std::cout << c2->position.z << std::endl;

	physicsSystem.Update(0.0166f, entities, messages, globalQueue);
	// std::cout << c1->position.x << std::endl;
	// std::cout << c1->position.y << std::endl;
	// std::cout << c1->position.z << std::endl;
}