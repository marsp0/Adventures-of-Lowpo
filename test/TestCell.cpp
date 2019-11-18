#include "catch.hpp"

#include "../src/Systems/Physics/Cell.hpp"

TEST_CASE("Test Cell")
{
	// Cell cell(glm::vec3(10.f,10.f,10.f), 5.f, 1, 1);
	// // box
	// std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, glm::vec3(0.f,0.f,0.f), ColliderType::BOX, DynamicType::Dynamic, glm::vec3(0.f,0.f,0.f)));
	// std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(3, glm::vec3(0.f,0.f,0.f), ColliderType::BOX, DynamicType::WithPhysics, glm::vec3(0.f,0.f,0.f)));
	// // plane
	// std::vector<glm::vec3> points1;
	// points1.push_back(glm::vec3(1.8f, 4.f, 0.f));
	// points1.push_back(glm::vec3(1.8f, 4.f, 4.f));
	// points1.push_back(glm::vec3(1.8f, 0.f, 4.f));
	// points1.push_back(glm::vec3(1.8f, 0.f, 0.f));
	// glm::vec3 center1 = glm::vec3(1.8f, 2.f, 2.f);
	// std::shared_ptr<Plane> plane1 = std::make_shared<Plane>(Plane(1, center1, ColliderType::PLANE, DynamicType::Static, points1));

	// cell.Insert(box1);
	// cell.Insert(box2);
	// cell.Insert(plane1);
	// REQUIRE(cell.GetDynamicColliders().size() == 2);
	// REQUIRE(cell.GetStaticColliders().size() == 1);
	// cell.Remove(box1);
	// REQUIRE(cell.GetStaticColliders().size() == 1);
	// REQUIRE(cell.GetDynamicColliders().size() == 1);
}