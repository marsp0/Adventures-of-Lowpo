#include <iostream>
#include <cmath>
#include "catch.hpp"
#include "../src/Systems/Physics/PhysicsSystem.hpp"
#include "../src/Systems/Physics/Plane.hpp"
#include "../src/Systems/Physics/AABB.hpp"
#include "../src/Components/PhysicsComponent.hpp"

TEST_CASE("PhysicsSystem Test")
{
	PhysicsSystem physicsSystem(200.f, 10.f);

	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(1.f, 1.f, 1.f));
	points1.push_back(glm::vec3(1.f, 3.f, 1.f));
	points1.push_back(glm::vec3(3.f, 3.f, 3.f));
	points1.push_back(glm::vec3(3.f, 1.f, 3.f));
	glm::vec3 center1 = glm::vec3(2.f, 2.f, 2.f);
	std::shared_ptr<Plane> plane1 = std::make_shared<Plane>(Plane(1, center1, ColliderType::PLANE, DynamicType::Static, points1));
	// std::shared_ptr<PhysicsComponent> component1 = std::make_shared<PhysicsComponent>(PhysicsComponent(1000000.f, center1, ))

	glm::vec3 box_center1 = glm::vec3(4.f, 2.f, 2.f);
	glm::vec3 axisRadii1 = glm::vec3(2.f,2.f,2.f);
	float xSquared = std::pow(axisRadii1.x * 2.f, 2.f);
	float ySquared = std::pow(axisRadii1.y * 2.f, 2.f);
	float zSquared = std::pow(axisRadii1.z * 2.f, 2.f);
	float mass = 4.f;
	float coeff = (1.f/12.f)*mass;

	float xInertia = coeff*(ySquared + zSquared);
	float yInertia = coeff*(xSquared + zSquared);
	float zInertia = coeff*(xSquared + ySquared);
	glm::mat3 inertiaTensorBox = glm::mat3(
		xInertia, 0.f, 0.f,
		0.f, yInertia, 0.f,
		0.f, 0.f, zInertia);
	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, box_center1, ColliderType::BOX, DynamicType::Dynamic, axisRadii1));
	glm::quat orientation(1.0, 0.f, 0.f, 0.f);
	std::shared_ptr<PhysicsComponent> component1 = std::make_shared<PhysicsComponent>(PhysicsComponent(mass, box_center1, orientation, inertiaTensorBox, DynamicType::WithPhysics));
}