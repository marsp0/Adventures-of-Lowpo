#include <iostream>
#include <cmath>
#include "catch.hpp"
#include "../src/Systems/Physics/Grid.hpp"
#include "../src/Systems/Physics/Plane.hpp"

TEST_CASE("Grid Test")
{
	Grid grid(200.f, 5.f);
	const std::vector< std::vector< std::shared_ptr<Cell>> > cells = grid.GetCells();

	REQUIRE(cells.size() == 20);
	REQUIRE(cells[0].size() == 20);

	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(1.f, 1.f, 1.f));
	points1.push_back(glm::vec3(1.f, 3.f, 1.f));
	points1.push_back(glm::vec3(3.f, 3.f, 3.f));
	points1.push_back(glm::vec3(3.f, 1.f, 3.f));
	glm::vec3 center1 = glm::vec3(2.f, 2.f, 2.f);
	std::shared_ptr<Plane> plane1 = std::make_shared<Plane>(Plane(1, center1, ColliderType::PLANE, DynamicType::Static, points1));

	std::vector<glm::vec3> points2;
	points2.push_back(glm::vec3(0.f, 0.f, 10.f));
	points2.push_back(glm::vec3(5.f, 0.f, 10.f));
	points2.push_back(glm::vec3(5.f, 0.f, 20.f));
	points2.push_back(glm::vec3(0.f, 0.f, 20.f));
	glm::vec3 center2 = glm::vec3(2.5f, 0.f, 15.f);
	std::shared_ptr<Plane> plane2 = std::make_shared<Plane>(Plane(2, center2, ColliderType::PLANE, DynamicType::Static, points2));

	glm::vec3 box_center1 = glm::vec3(4.f, 2.f, 2.f);
	glm::vec3 axisRadii1 = glm::vec3(2.f,2.f,2.f);
	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, box_center1, ColliderType::BOX, DynamicType::Dynamic, axisRadii1));

	glm::vec3 box_center2 = glm::vec3(17.5f, 2.5f, 2.5f);
	glm::vec3 axisRadii2 = glm::vec3(2.5f,2.5f,2.5f);
	std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(4, box_center2, ColliderType::BOX, DynamicType::Dynamic, axisRadii2));

	glm::vec3 box_center3 = glm::vec3(17.5f, 3.5f, 6.5f);
	std::shared_ptr<AABB> box3 = std::make_shared<AABB>(AABB(5, box_center3, ColliderType::BOX, DynamicType::Dynamic, axisRadii2));

	grid.Insert(plane1);
	grid.Insert(plane2);
	grid.Insert(box1);
	grid.Insert(box2);
	grid.Insert(box3);

	SECTION("Test insertion")
	{
		std::shared_ptr<Cell> cell1 = grid.GetCells()[0][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders1 = cell1->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders1 = cell1->GetStaticColliders();
		REQUIRE(dynamicColliders1.size() == 1);
		REQUIRE(staticColliders1.size() == 1);

		std::shared_ptr<Cell> cell2 = grid.GetCells()[1][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders2 = cell2->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders2 = cell2->GetStaticColliders();
		REQUIRE(dynamicColliders2.size() == 0);
		REQUIRE(staticColliders2.size() == 1);

		std::shared_ptr<Cell> cell3 = grid.GetCells()[0][1];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders3 = cell3->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders3 = cell3->GetStaticColliders();
		REQUIRE(dynamicColliders3.size() == 2);
		REQUIRE(staticColliders3.size() == 0);

		std::shared_ptr<Cell> cell4 = grid.GetCells()[1][1];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders4 = cell4->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders4 = cell4->GetStaticColliders();
		REQUIRE(dynamicColliders4.size() == 0);
		REQUIRE(staticColliders4.size() == 0);

	}

	SECTION("Test Removal")
	{
		grid.Remove(plane1);
		grid.Remove(box1);
		std::shared_ptr<Cell> cell1 = grid.GetCells()[0][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders1 = cell1->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders1 = cell1->GetStaticColliders();
		REQUIRE(dynamicColliders1.size() == 0);
		REQUIRE(staticColliders1.size() == 0);
	}

	SECTION("Test GetEligibleCells")
	{
		std::vector<std::pair<int, int>> eligibleCells = grid.GetEligibleCells(0, 0);
		REQUIRE(eligibleCells.size() == 4);
		std::vector<std::pair<int, int>> eligibleCells1 = grid.GetEligibleCells(4, 4);
		REQUIRE(eligibleCells1.size() == 6);
		std::vector<std::pair<int, int>> eligibleCells2 = grid.GetEligibleCells(19, 19);
		REQUIRE(eligibleCells2.size() == 2);
		std::vector<std::pair<int, int>> eligibleCells3 = grid.GetEligibleCells(19, 0);
		REQUIRE(eligibleCells3.size() == 4);
	}

	SECTION("Test collisions")
	{
		std::vector<std::shared_ptr<Collision>> collisions = grid.CheckCollisions();
		REQUIRE(collisions.size() == 2);

		std::shared_ptr<Collision> collision1 = collisions[0];
		REQUIRE(collision1->first == 3);
		REQUIRE(collision1->second == 1);
		REQUIRE(collision1->firstCollider->entityID == 3);
		REQUIRE(collision1->secondCollider->entityID == 1);
		REQUIRE(collision1->contacts.size() == 2);
		for (int i = 0; i < collision1->contacts.size(); i++)
		{
			REQUIRE(collision1->contacts[i].penetration - 1.0f < 0.00005f);
			REQUIRE(glm::dot(collision1->contacts[i].contactNormal, glm::vec3(-1.f,0.f,0.f)) - 1.f < 0.00005f);
		}
		REQUIRE(collision1->contacts[0].contactPoint.x == 3.f);
		REQUIRE(collision1->contacts[0].contactPoint.y == 3.f);
		REQUIRE(collision1->contacts[0].contactPoint.z == 3.f);

		REQUIRE(collision1->contacts[1].contactPoint.x == 3.f);
		REQUIRE(collision1->contacts[1].contactPoint.y == 1.f);
		REQUIRE(collision1->contacts[1].contactPoint.z == 3.f);

		std::shared_ptr<Collision> collision2 = collisions[1];
		glm::vec3 normal = glm::vec3(0.f, 0.f, 1.f);
		REQUIRE(collision2->first == 4);
		REQUIRE(collision2->second == 5);
		REQUIRE(collision2->firstCollider->entityID == 4);
		REQUIRE(collision2->secondCollider->entityID == 5);
		REQUIRE(collision2->contacts.size() == 2);
		for (int i = 0; i < collision2->contacts.size(); i++)
		{
			REQUIRE(collision2->contacts[i].penetration - 1.0f < 0.00005f);
			REQUIRE(fabs(glm::dot(collision2->contacts[i].contactNormal, normal)) == 1.f);
		}
	}
}