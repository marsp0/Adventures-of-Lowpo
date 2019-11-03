#include "catch.hpp"
#include <memory>
#include <iostream>
#include "../src/Systems/Physics/Plane.hpp"
#include "../src/Systems/Physics/AABB.hpp"
#include "../src/Systems/Physics/CollisionDetector.hpp"

TEST_CASE("CollisionDetector Test")
{
	CollisionDetector detector = CollisionDetector();
	// setup
	// to test
	// 1. contact normal direction
	// 2. different angled collisions and if they produce the right results.

	// types of collisions
	// BOX Face to Plane face - box1 and plane1
	// Box Face to Plane edge - box1 and plane2
	// box edge to plane edge - box1 and plane3
	// Box face to box face - box1 and box2

	std::vector<glm::vec3> pointsPlane1;
	pointsPlane1.push_back(glm::vec3(-1.f, 0.f, 0.05f));
	pointsPlane1.push_back(glm::vec3(4.f, 0.f, 0.05f));
	pointsPlane1.push_back(glm::vec3(-1.f, 4.f, 0.05f));
	pointsPlane1.push_back(glm::vec3(4.f, 4.f, 0.05f));
	std::shared_ptr<Plane> plane1 = std::make_shared<Plane>(Plane(1, glm::vec3(1.5f, 2.f, 0.f),
																	ColliderType::PLANE,
																	DynamicType::Static,
																	pointsPlane1));

	std::vector<glm::vec3> pointsPlane2;
	pointsPlane2.push_back(glm::vec3(1.8f, 1.72f, 2.82f));
	pointsPlane2.push_back(glm::vec3(1.8f, 1.72f, 1.82f));
	pointsPlane2.push_back(glm::vec3(0.8f, 1.f, 2.82f));
	pointsPlane2.push_back(glm::vec3(0.8f, 1.f, 1.82f));
	std::shared_ptr<Plane> plane2 = std::make_shared<Plane>(Plane(444, glm::vec3(1.3f, 1.3f, 2.32f),
																	ColliderType::PLANE,
																	DynamicType::Static,
																	pointsPlane2));

	std::vector<glm::vec3> pointsPlane3;
	pointsPlane3.push_back(glm::vec3(3.f, 1.f, 1.f));
	pointsPlane3.push_back(glm::vec3(0.9f, -1.f, 1.f));
	pointsPlane3.push_back(glm::vec3(2.f, -2.f, 1.f));
	pointsPlane3.push_back(glm::vec3(4.f, 0.f, 1.f));
	std::shared_ptr<Plane> plane3 = std::make_shared<Plane>(Plane(33, glm::vec3(2.5f, -0.5f, 1.f),
																	ColliderType::PLANE,
																	DynamicType::Static,
																	pointsPlane3));	

	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(4, glm::vec3(1.f, 1.f, 1.f), 
																ColliderType::BOX, 
																DynamicType::Dynamic,
																glm::vec3(1.f, 1.f, 1.f)));

	std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(666, glm::vec3(-0.9f, 1.1f, 1.1f),
																ColliderType::BOX,
																DynamicType::Dynamic,
																glm::vec3(1.f, 1.f, 1.f)));

	SECTION("Box Faces - Plane Face")
	{
		std::shared_ptr<Collision> collision1 = detector.CheckCollision(box1, plane1);
		REQUIRE(collision1 != nullptr);
		REQUIRE(collision1->contacts.size() == 4);
		float dotResult = glm::dot(collision1->contacts[0].contactNormal,glm::vec3(0.f, 0.f, 1.f)) - 1.f;
		REQUIRE( dotResult < 0.0000005f);
		REQUIRE( dotResult >= 0.f);
	}
	SECTION("Box Face - Plane edge")
	{
		std::shared_ptr<Collision> collision2 = detector.CheckCollision(box1, plane2);
		REQUIRE(collision2 != nullptr);
		REQUIRE(collision2->contacts.size() == 2);
		float dotResult = glm::dot(collision2->contacts[0].contactNormal,glm::vec3(0.f, 0.f, 1.f)) + 1.f;
		REQUIRE( dotResult < 0.0000005f);
		REQUIRE( dotResult >= 0.f);
	}
	SECTION("Box Edge - Plane edge")
	{
		std::shared_ptr<Collision> collision2 = detector.CheckCollision(box1, plane3);
		REQUIRE(collision2 != nullptr);
		REQUIRE(collision2->contacts.size() == 1);
		glm::vec3 edgeA = glm::vec3(3.f, 1.f, 1.f) - glm::vec3(0.9f, -1.f, 1.f);
		glm::vec3 edgeB = glm::vec3(2.f, 0.f, 2.f) - glm::vec3(2.f, 0.f, 0.f);
		glm::vec3 expectedNormal = glm::normalize(glm::cross(edgeA, edgeB));
		float dotResult = glm::dot(collision2->contacts[0].contactNormal,expectedNormal) + 1.f;
		REQUIRE( dotResult < 0.0000005f);
		REQUIRE( dotResult >= 0.f);
	}
	SECTION("Box Face - Plane Face")
	{
		std::shared_ptr<Collision> collision2 = detector.CheckCollision(box1, box2);
		REQUIRE(collision2 != nullptr);
		REQUIRE(collision2->contacts.size() == 4);
		float dotResult = glm::dot(collision2->contacts[0].contactNormal, glm::vec3(1.f, 0.f, 0.f)) - 1.f;
		REQUIRE( dotResult < 0.0000005f);
		REQUIRE( dotResult >= 0.f);
	}
}