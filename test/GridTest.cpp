#include <iostream>
#include "catch.hpp"
#include "../src/Systems/Physics/Grid.hpp"

TEST_CASE("Grid Test")
{
	float gridLen = 200.f;
	float halfWidth = 10.f;
	Grid grid(gridLen, halfWidth);
	const std::vector< std::vector< std::shared_ptr<Cell>> > cells = grid.GetCells();
	REQUIRE(cells.size() == 10);
	REQUIRE(cells[0].size() == 10);

	std::vector<glm::vec3> points1{glm::vec3(18.f,0.f,7.34f),glm::vec3(22.f,0.f,6.61f),glm::vec3(13.f,22.f,0.f)};
	std::vector<glm::vec3> points2{glm::vec3(50.f,0.f,60.f),glm::vec3(60.f,0.f,50.f),glm::vec3(50.f,0.f,50.f)};
	std::shared_ptr<Triangle> triangle1 = std::make_shared<Triangle>(Triangle(1, glm::vec3(17.666f,7.33f, 12.f), "triangle1", glm::vec3(0.f,0.633f,0.773f), points1, ColliderType::TRIANGLE, DynamicType::Static));
	std::shared_ptr<Triangle> triangle2 = std::make_shared<Triangle>(Triangle(2, glm::vec3(53.3333f,0.f,53.333f), "triangle2", glm::vec3(0.f,1.f,0.f), points2, ColliderType::TRIANGLE, DynamicType::Static));
	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, glm::vec3(18.f,10.f,17.f), "box1", glm::vec3(10.f,10.f,10.f), ColliderType::BOX, DynamicType::Dynamic));
	std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(4, glm::vec3(35.f,10.f,35.f), "box2", glm::vec3(10.f,10.f,10.f), ColliderType::BOX, DynamicType::WithPhysics));

	grid.Insert(triangle1);
	grid.Insert(triangle2);
	grid.Insert(box1);
	grid.Insert(box2);

	SECTION("Insertion Test")
	{
		std::shared_ptr<Cell> cell = cells[0][0];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders = cell->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders = cell->GetStaticColliders();
		REQUIRE(dynamicColliders.size() == 1);
		REQUIRE(dynamicColliders[0]->name == "box1");
		REQUIRE(staticColliders.size() == 1);
		REQUIRE(staticColliders[0]->name == "triangle1");

		cell = cells[1][1];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders2 = cell->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders2 = cell->GetStaticColliders();
		REQUIRE(dynamicColliders2.size() == 1);
		REQUIRE(staticColliders2.size() == 0);

		cell = cells[2][2];
		const std::vector<std::shared_ptr<Collider>> dynamicColliders3 = cell->GetDynamicColliders();
		const std::vector<std::shared_ptr<Collider>> staticColliders3 = cell->GetStaticColliders();
		REQUIRE(dynamicColliders3.size() == 0);
		REQUIRE(staticColliders3.size() == 1);
	}

	SECTION("Collision Check")
	{
		std::vector<std::shared_ptr<Collision>> collisions = grid.CheckCollisions();
		REQUIRE(collisions.size() == 2);
		std::shared_ptr<Collision> b1t1 = collisions[0];
		REQUIRE(b1t1->first == 3);
		REQUIRE(b1t1->second == 1);

		b1t1 = collisions[1];
		REQUIRE(b1t1->first == 3);
		REQUIRE(b1t1->second == 4);
	}

}

TEST_CASE("Box - Box")
{
	float epsilon = 0.00005f;
	float gridLen = 200.f;
	float halfWidth = 10.f;
	Grid grid(gridLen, halfWidth);
	const std::vector< std::vector< std::shared_ptr<Cell>> > cells = grid.GetCells();
	REQUIRE(cells.size() == 10);
	REQUIRE(cells[0].size() == 10);

	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(1, glm::vec3(18.f,10.f,17.f), "box1", glm::vec3(10.f,10.f,10.f), ColliderType::BOX, DynamicType::Dynamic));
	std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(2, glm::vec3(30.f,7.f,15.f), "box2", glm::vec3(5.f,5.f,5.f), ColliderType::BOX, DynamicType::WithPhysics));
	grid.Insert(box1);
	grid.Insert(box2);
	SECTION("Face - Face")
	{
		std::vector<std::shared_ptr<Collision>> collisions = grid.CheckCollisions();
		REQUIRE(collisions.size() == 1);
		
		std::shared_ptr<Collision> collision = collisions[0];
		REQUIRE(collision->first == 1);
		REQUIRE(collision->second == 2);

		std::vector<Contact> contacts = collision->contacts;
		REQUIRE(contacts.size() == 4);

		for (int i = 0; i < contacts.size(); i++)
		{
			REQUIRE(glm::dot(contacts[i].contactNormal, glm::vec3(1.f,0.f, 0.f)) - 1.f <= epsilon);
			REQUIRE(contacts[i].penetration - 3.f <= epsilon);
		}
	}
}