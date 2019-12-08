#include <iostream>
#include <vector>
#include <cmath>
#include "catch.hpp"
#include "../src/Systems/Physics/PhysicsSystem.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"
#include "../src/Components/PhysicsComponent.hpp"
#include "../src/Components/TransformComponent.hpp"
#include "../src/util.hpp"

TEST_CASE("PhysicsSystem Test")
{
	PhysicsSystem physicsSystem(200.f, 10.f);
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(0.f, 0.f, 0.f)); // 0
	points1.push_back(glm::vec3(2.f, 0.f, 0.f)); // 1
	points1.push_back(glm::vec3(2.f, 2.f, 0.f)); // 2
	points1.push_back(glm::vec3(0.f, 2.f, 0.f)); // 3
	points1.push_back(glm::vec3(0.f, 0.f, 2.f)); // 4
	points1.push_back(glm::vec3(2.f, 0.f, 2.f)); // 5
	points1.push_back(glm::vec3(2.f, 2.f, 2.f)); // 6
	points1.push_back(glm::vec3(0.f, 2.f, 2.f)); // 7
	std::shared_ptr<Collider> collider1 = ColliderBuilder::Build(1, DynamicType::Static, points1);

	std::vector<glm::vec3> points2;
	points2.push_back(glm::vec3(3.f, 1.5f, 1.95f));
	points2.push_back(glm::vec3(3.f, 1.f, 1.95f));
	points2.push_back(glm::vec3(-1.f, 1.5f, 1.95f));
	points2.push_back(glm::vec3(-1.f, 1.f, 1.95f));
	points2.push_back(glm::vec3(3.f, 1.5f, 2.1f));
	points2.push_back(glm::vec3(3.f, 1.f, 2.1f));
	points2.push_back(glm::vec3(-1.f, 1.5f, 2.1f));
	points2.push_back(glm::vec3(-1.f, 1.f, 2.1f));
	std::shared_ptr<Collider> collider2 = ColliderBuilder::Build(2, DynamicType::Dynamic, points2);

	glm::quat orientation(1.0, 0.f, 0.f, 0.f);
	float mass = 1000000.f;
	float coeff = (1.f/12.f)*mass;
	glm::mat3 inertiaTensor = glm::mat3(
		coeff*4, 0.f, 0.f,
		0.f, coeff*4, 0.f,
		0.f, 0.f, coeff*8);
	std::unique_ptr<PhysicsComponent> component1 = std::make_unique<PhysicsComponent>(mass, collider1->center, orientation, inertiaTensor, DynamicType::Static);
	std::unique_ptr<TransformComponent> transformComponent1 = std::make_unique<TransformComponent>(collider1->center, orientation);
	component1->colliders.push_back(collider1);
	std::unique_ptr<Entity> entity1 = std::make_unique<Entity>(1);
	entity1->AddComponent(std::move(component1));
	entity1->AddComponent(std::move(transformComponent1));

	float xSquared = std::pow(4.f * 2.f, 2.f);
	float ySquared = std::pow(.0625f * 2.f, 2.f);
	float zSquared = std::pow(.005625f * 2.f, 2.f);
	mass = 4.f;
	coeff = (1.f/12.f)*mass;

	glm::mat3 inertiaTensorBox = glm::mat3(
		coeff*(ySquared + zSquared), 0.f, 0.f,
		0.f, coeff*(xSquared + zSquared), 0.f,
		0.f, 0.f, coeff*(xSquared + ySquared));
	std::unique_ptr<PhysicsComponent> component2 = std::make_unique<PhysicsComponent>(mass, collider2->center, orientation, inertiaTensorBox, DynamicType::WithPhysics);
	std::unique_ptr<TransformComponent> transformComponent2 = std::make_unique<TransformComponent>(collider2->center, orientation);
	component2->velocity = glm::vec3(0.f, 1.f, -8.f);
	component2->colliders.push_back(collider2);
	std::unique_ptr<Entity> entity2 = std::make_unique<Entity>(2);
	entity2->AddComponent(std::move(component2));
	entity2->AddComponent(std::move(transformComponent2));
	std::vector<std::shared_ptr<Collider>> colliders{collider1, collider2};
	physicsSystem.Insert(colliders);
	std::vector<std::unique_ptr<Entity>> entities;
	entities.push_back(std::move(entity1));
	entities.push_back(std::move(entity2));
	std::vector<Message> messages;
	std::vector<Message> globalQueue;



	PhysicsComponent* component = entities[1]->GetComponent<PhysicsComponent>(ComponentType::Physics);
	printVector(component->position, "PRE UPDATE Position");
	printVector(component->velocity, "PRE UPDATE VEL");

	physicsSystem.Update(0.0166f, entities, messages, globalQueue);

	printVector(component->position, "POST UPDATE Position");
	printVector(component->velocity, "POSTUPDATE VEL");

	physicsSystem.Update(0.0166f, entities, messages, globalQueue);

	printVector(component->position, "POST UPDATE Position");
	printVector(component->velocity, "POSTUPDATE VEL");

}