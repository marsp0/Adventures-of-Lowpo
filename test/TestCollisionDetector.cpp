#include "catch.hpp"
#include <memory>
#include <iostream>
#include "../src/Systems/Physics/Collider.hpp"
#include "../src/Systems/Physics/CollisionDetector.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"
#include "../src/util.hpp"

TEST_CASE("CollisionDetector Test")
{
	CollisionDetector detector = CollisionDetector();
	// setup
	// to test
	// 1. contact normal direction
	// 2. different angled collisions and if they produce the right results.

	// types of collisions
	// BOX Face to Box face - box1 and plane1
	// Box Face to Plane edge - box1 and plane2
	// box edge to plane edge - box1 and plane3
	// Box face to box face - box1 and box2

	std::vector<std::pair<glm::vec3, glm::vec3>> edges1;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces1;
	glm::vec3 center1;
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(-1.f, -0.1f, 0.05f));
	points1.push_back(glm::vec3(4.f, -0.1f, 0.05f));
	points1.push_back(glm::vec3(-1.f, 4.f, 0.05f));
	points1.push_back(glm::vec3(4.f, 4.f, 0.05f));

	points1.push_back(glm::vec3(-1.f, -0.1f, -0.2f));
	points1.push_back(glm::vec3(4.f, -0.1f, -0.2f));
	points1.push_back(glm::vec3(-1.f, 4.f, -0.2f));
	points1.push_back(glm::vec3(4.f, 4.f, -0.2f));
	ColliderBuilder::Build(points1, faces1, edges1, center1);
	std::shared_ptr<Collider> collider1 = std::make_shared<Collider>(1, center1, points1, edges1, faces1, DynamicType::Static);


	std::vector<std::pair<glm::vec3, glm::vec3>> edges2;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces2;
	glm::vec3 center2;
	std::vector<glm::vec3> points2;
	points2.push_back(glm::vec3(1.8f, 1.7f, 2.8f));
	points2.push_back(glm::vec3(1.8f, 1.7f, 1.8f));
	points2.push_back(glm::vec3(0.8f, 1.f, 2.8f));
	points2.push_back(glm::vec3(0.8f, 1.f, 1.8f));

	points2.push_back(glm::vec3(1.3f, 2.2f, 2.8f));
	points2.push_back(glm::vec3(1.3f, 2.2f, 1.8f));
	points2.push_back(glm::vec3(0.3f, 1.5f, 2.8f));
	points2.push_back(glm::vec3(0.3f, 1.5f, 1.8f));
	ColliderBuilder::Build(points2, faces2, edges2, center2);
	std::shared_ptr<Collider> collider2 = std::make_shared<Collider>(2, center2, points2, edges2, faces2, DynamicType::Static);
	

	std::vector<std::pair<glm::vec3, glm::vec3>> edges3;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces3;
	glm::vec3 center3;
	std::vector<glm::vec3> points3;
	points3.push_back(glm::vec3(3.f, 1.f, 1.f));
	points3.push_back(glm::vec3(0.9f, -1.f, 1.f));
	points3.push_back(glm::vec3(2.f, -2.f, 1.f));
	points3.push_back(glm::vec3(4.f, 0.f, 1.f));

	points3.push_back(glm::vec3(3.f, 1.f, 0.f));
	points3.push_back(glm::vec3(0.9f, -1.f, 0.f));
	points3.push_back(glm::vec3(2.f, -2.f, 0.f));
	points3.push_back(glm::vec3(4.f, 0.f, 0.f));
	ColliderBuilder::Build(points3, faces3, edges3, center3);
	std::shared_ptr<Collider> collider3 = std::make_shared<Collider>(3, center3, points3, edges3, faces3, DynamicType::Static);
		

	std::vector<std::pair<glm::vec3, glm::vec3>> edges4;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces4;
	glm::vec3 center4;
	std::vector<glm::vec3> points4;
	points4.push_back(glm::vec3(0.f, 0.f, 0.f));
	points4.push_back(glm::vec3(2.f, 0.f, 0.f));
	points4.push_back(glm::vec3(2.f, 2.f, 0.f));
	points4.push_back(glm::vec3(0.f, 2.f, 0.f));

	points4.push_back(glm::vec3(0.f, 0.f, 2.f));
	points4.push_back(glm::vec3(2.f, 0.f, 2.f));
	points4.push_back(glm::vec3(2.f, 2.f, 2.f));
	points4.push_back(glm::vec3(0.f, 2.f, 2.f));
	
	ColliderBuilder::Build(points4, faces4, edges4, center4);
	std::shared_ptr<Collider> collider4 = std::make_shared<Collider>(4, center4, points4, edges4, faces4, DynamicType::Dynamic);
	

	std::vector<std::pair<glm::vec3, glm::vec3>> edges5;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces5;
	glm::vec3 center5;
	std::vector<glm::vec3> points5;
	points5.push_back(glm::vec3(-1.9f, 0.1f, 0.1f));
	points5.push_back(glm::vec3(0.1f, 0.1f, 0.1f));
	points5.push_back(glm::vec3(-1.9f, 2.1f, 0.1f));
	points5.push_back(glm::vec3(0.1f, 2.1f, 0.1f));

	points5.push_back(glm::vec3(-1.9f, 0.1f, 2.1f));
	points5.push_back(glm::vec3(0.1f, 0.1f, 2.1f));
	points5.push_back(glm::vec3(-1.9f, 2.1f, 2.1f));
	points5.push_back(glm::vec3(0.1f, 2.1f, 2.1f));
	
	ColliderBuilder::Build(points5, faces5, edges5, center5);
	std::shared_ptr<Collider> collider5 = std::make_shared<Collider>(5, center5, points5, edges5, faces5, DynamicType::Dynamic);
	

	// SECTION("Box Faces - Box Face")
	// {
	// 	std::shared_ptr<Collision> collision1 = detector.CheckCollision(collider4, collider1);
	// 	REQUIRE(collision1 != nullptr);
	// 	REQUIRE(collision1->contacts.size() == 4);
	// 	float dotResult = 1.f - glm::dot(collision1->contacts[0].contactNormal,glm::vec3(0.f, 0.f, 1.f));
	// 	REQUIRE( dotResult < 0.0000005f);
	// 	REQUIRE( dotResult >= 0.f);
	// }
	SECTION("Box Face - Plane edge")
	{
		std::shared_ptr<Collision> collision2 = detector.CheckCollision(collider4, collider2);
		REQUIRE(collision2 != nullptr);
		// REQUIRE(collision2->contacts.size() == 2);
		for (int i = 0; i < collision2->contacts.size(); ++i)
		{
			printVector(collision2->contacts[i].contactPoint);
		}
		float dotResult = glm::dot(collision2->contacts[0].contactNormal,glm::vec3(0.f, 0.f, 1.f)) + 1.f;
		REQUIRE( dotResult < 0.0000005f);
		REQUIRE( dotResult >= 0.f);
	}
	// SECTION("Box Edge - Plane edge")
	// {
	// 	std::shared_ptr<Collision> collision2 = detector.CheckCollision(collider4, collider3);
	// 	REQUIRE(collision2 != nullptr);
	// 	REQUIRE(collision2->contacts.size() == 1);
	// 	glm::vec3 edgeA = glm::vec3(3.f, 1.f, 1.f) - glm::vec3(0.9f, -1.f, 1.f);
	// 	glm::vec3 edgeB = glm::vec3(2.f, 0.f, 2.f) - glm::vec3(2.f, 0.f, 0.f);
	// 	glm::vec3 expectedNormal = glm::normalize(glm::cross(edgeA, edgeB));
	// 	float dotResult = glm::dot(collision2->contacts[0].contactNormal,expectedNormal) + 1.f;
	// 	REQUIRE( dotResult < 0.0000005f);
	// 	REQUIRE( dotResult >= 0.f);
	// }
	// SECTION("Box Face - Plane Face")
	// {
	// 	std::shared_ptr<Collision> collision2 = detector.CheckCollision(collider4, collider5);
	// 	REQUIRE(collision2 != nullptr);
	// 	REQUIRE(collision2->contacts.size() == 4);
	// 	float dotResult = glm::dot(collision2->contacts[0].contactNormal, glm::vec3(1.f, 0.f, 0.f)) - 1.f;
	// 	REQUIRE( dotResult < 0.0000005f);
	// 	REQUIRE( dotResult >= 0.f);
	// }
}