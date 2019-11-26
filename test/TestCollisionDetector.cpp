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

	/*
		types of collisions - Currently we are only able to work with BOXES
		face - vertex
		face - edge
		face - face
		edge - edge
		big face - small face
		small face - big face
	*/

	std::vector<std::pair<glm::vec3, glm::vec3>> edges1;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces1;
	glm::vec3 center1;
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(0.f, 0.f, 0.f));
	points1.push_back(glm::vec3(2.f, 0.f, 0.f));
	points1.push_back(glm::vec3(2.f, 2.f, 0.f));
	points1.push_back(glm::vec3(0.f, 2.f, 0.f));
	points1.push_back(glm::vec3(0.f, 0.f, 2.f));
	points1.push_back(glm::vec3(2.f, 0.f, 2.f));
	points1.push_back(glm::vec3(2.f, 2.f, 2.f));
	points1.push_back(glm::vec3(0.f, 2.f, 2.f));
	ColliderBuilder::Build(points1, faces1, edges1, center1);
	std::shared_ptr<Collider> collider1 = std::make_shared<Collider>(1, center1, points1, edges1, faces1, DynamicType::Static);

	std::vector<std::pair<glm::vec3, glm::vec3>> edges2;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces2;
	glm::vec3 center2;
	std::vector<glm::vec3> points2;
	points2.push_back(glm::vec3(1.5f, 1.f, 0.5f));
	points2.push_back(glm::vec3(3.f, 1.f, 0.f));
	points2.push_back(glm::vec3(3.f, 2.58f, 0.f));
	points2.push_back(glm::vec3(1.5f, 2.58f, 0.5f));
	points2.push_back(glm::vec3(3.5f, 1.f, 1.5f));
	points2.push_back(glm::vec3(2.f, 2.58f, 2.f));
	points2.push_back(glm::vec3(2.f, 1.f, 2.f));
	points2.push_back(glm::vec3(3.5f, 2.58f, 1.5f));
	ColliderBuilder::Build(points2, faces2, edges2, center2);
	std::shared_ptr<Collider> collider2 = std::make_shared<Collider>(2, center2, points2, edges2, faces2, DynamicType::Static);

	std::vector<std::pair<glm::vec3, glm::vec3>> edges3;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces3;
	glm::vec3 center3;
	std::vector<glm::vec3> points3;
	points3.push_back(glm::vec3(1.f, 1.75f, 0.5f));
	points3.push_back(glm::vec3(1.f, 1.75f, 1.5f));
	points3.push_back(glm::vec3(1.69f, 2.47f, 0.5f));
	points3.push_back(glm::vec3(.97f, 3.16f, .5f));
	points3.push_back(glm::vec3(.28f, 2.44f, 0.5f));
	points3.push_back(glm::vec3(1.69f, 2.47f, 1.5f));
	points3.push_back(glm::vec3(.97f, 3.16f, 1.5f));
	points3.push_back(glm::vec3(.28f, 2.44f, 1.5f));
	ColliderBuilder::Build(points3, faces3, edges3, center3);
	std::shared_ptr<Collider> collider3 = std::make_shared<Collider>(3, center3, points3, edges3, faces3, DynamicType::Static);

	std::vector<std::pair<glm::vec3, glm::vec3>> edges4;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces4;
	glm::vec3 center4;
	std::vector<glm::vec3> points4;
	points4.push_back(glm::vec3(0.f, 1.5f, 0.f));
	points4.push_back(glm::vec3(2.f, 1.5f, 0.f));
	points4.push_back(glm::vec3(2.f, 3.5f, 0.f));
	points4.push_back(glm::vec3(0.f, 3.5f, 0.f));
	points4.push_back(glm::vec3(0.f, 1.5f, 2.f));
	points4.push_back(glm::vec3(2.f, 1.5f, 2.f));
	points4.push_back(glm::vec3(2.f, 3.5f, 2.f));
	points4.push_back(glm::vec3(0.f, 3.5f, 2.f));
	ColliderBuilder::Build(points4, faces4, edges4, center4);
	std::shared_ptr<Collider> collider4 = std::make_shared<Collider>(4, center4, points4, edges4, faces4, DynamicType::Static);\

	std::vector<std::pair<glm::vec3, glm::vec3>> edges5;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces5;
	glm::vec3 center5;
	std::vector<glm::vec3> points5;
	points5.push_back(glm::vec3(0.72f, -1.29f, 1.5f));
	points5.push_back(glm::vec3(3.f, 1.5f, 1.5f));
	points5.push_back(glm::vec3(2.74f, -2.94f, -0.98f));
	points5.push_back(glm::vec3(5.02f, -.16f, -0.98f));
	points5.push_back(glm::vec3(2.64f, -2.86f, 4.12f));
	points5.push_back(glm::vec3(4.92f, -.07f, 4.12f));
	points5.push_back(glm::vec3(4.66f, -4.51f, 1.64f));
	points5.push_back(glm::vec3(6.94f, -1.72f, 1.64f));
	ColliderBuilder::Build(points5, faces5, edges5, center5);
	std::shared_ptr<Collider> collider5 = std::make_shared<Collider>(5, center5, points5, edges5, faces5, DynamicType::Static);
	
	// SECTION("Collider 1/2 - face/vertex")
	// {
	// 	std::shared_ptr<Collision> collision1 = detector.Collide(collider1, collider2);
	// 	REQUIRE(collision1 != nullptr);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(-1.f, 0.f, 0.f), collision1->contacts[0].contactNormal, detector.tolerance)));
	// 	REQUIRE(collision1->contacts.size() == 2);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.5f, 1.f, 0.5f), collision1->contacts[0].contactPoint, detector.tolerance)));
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(2.f, 1.f, 2.f), collision1->contacts[1].contactPoint, detector.tolerance)));
	// }
	// SECTION("Collider 2/1 - face/vertex")
	// {
	// 	std::shared_ptr<Collision> collision1 = detector.Collide(collider2, collider1);
	// 	REQUIRE(collision1 != nullptr);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.f, 0.f, 0.f), collision1->contacts[0].contactNormal, detector.tolerance)));
	// 	REQUIRE(collision1->contacts.size() == 2);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.5f, 1.f, 0.5f), collision1->contacts[0].contactPoint, detector.tolerance)));
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(2.f, 1.f, 2.f), collision1->contacts[1].contactPoint, detector.tolerance)));
	// }
	// SECTION("Collider 1/3 - face/edge")
	// {
	// 	std::shared_ptr<Collision> collision2 = detector.Collide(collider1, collider3);
	// 	REQUIRE(collision2 != nullptr);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(0.f, -1.f, 0.f), collision2->contacts[0].contactNormal, detector.tolerance)));
	// 	REQUIRE(collision2->contacts.size() == 2);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.f, 1.75f, 1.5f), collision2->contacts[1].contactPoint, detector.tolerance)));
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.f, 1.75f, .5f), collision2->contacts[0].contactPoint, detector.tolerance)));
	// }
	// SECTION("Collider 3/1 - face/edge")
	// {
	// 	std::shared_ptr<Collision> collision2 = detector.Collide(collider3, collider1);
	// 	REQUIRE(collision2 != nullptr);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(0.f, 1.f, 0.f), collision2->contacts[0].contactNormal, detector.tolerance)));
	// 	REQUIRE(collision2->contacts.size() == 2);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.f, 1.75f, 1.5f), collision2->contacts[1].contactPoint, detector.tolerance)));
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(1.f, 1.75f, .5f), collision2->contacts[0].contactPoint, detector.tolerance)));
	// }

	// SECTION("Collider 1/4 - face/face")
	// {
	// 	std::shared_ptr<Collision> collision = detector.Collide(collider1, collider4);
	// 	REQUIRE(collision != nullptr);
	// 	REQUIRE(glm::all(glm::epsilonEqual(glm::vec3(0.f, -1.f, 0.f), collision->contacts[0].contactNormal, detector.tolerance)));
	// 	REQUIRE(collision->contacts.size() == 4);
	// 	std::vector<glm::vec3> expectedPoints;
	// 	expectedPoints.push_back(glm::vec3(2.f, 2.f, 0.f));
	// 	expectedPoints.push_back(glm::vec3(0.f, 2.f, 0.f));
	// 	expectedPoints.push_back(glm::vec3(0.f, 2.f, 2.f));
	// 	expectedPoints.push_back(glm::vec3(2.f, 2.f, 2.f));

	// 	for (int i = 0; i < expectedPoints.size(); i++)
	// 	{
	// 		bool found = false;
	// 		for (int j = 0; j < collision->contacts.size(); j++)
	// 		{
	// 			if (glm::all(glm::epsilonEqual(expectedPoints[i], collision->contacts[j].contactPoint, detector.tolerance)))
	// 				found = true;
	// 		}
	// 		REQUIRE(found == true);
	// 	}
	// }
	SECTION("Collider 1/5 - edge/edge")
	{
		std::shared_ptr<Collision> collision = detector.Collide(collider1, collider5);
		REQUIRE(collision != nullptr);
		REQUIRE(collision->contacts.size() == 1);
		glm::vec3 expectedNormal = glm::normalize(glm::cross(points1[1] - points1[5], points5[1] - points5[0]));
		// REQUIRE(glm::all(glm::epsilonEqual(expectedNormal, collision->contacts[0].contactNormal, detector.tolerance)));
		printVector(collision->contacts[0].contactNormal);
		for (int i = 0; i < collision->contacts.size(); ++i)
		{
			std::cout << collision->contacts[i].penetration << std::endl;
			printVector(collision->contacts[i].contactPoint);
		}
	}
}