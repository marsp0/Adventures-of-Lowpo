#include "catch.hpp"

#include "../src/Systems/Physics/Cell.hpp"
#include "../src/Systems/Physics/AABB.hpp"

TEST_CASE("Test Cell")
{
	glm::vec3 center = glm::vec3(10.f,10.f,10.f);
	float halfWidth = 5.f;
	int row = 1;
	int col = 1;
	Cell cell(center, halfWidth, row, col);
	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, glm::vec3(0.f,0.f,0.f), "box1", glm::vec3(0.f,0.f,0.f), ColliderType::BOX, DynamicType::Dynamic));
	std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(3, glm::vec3(0.f,0.f,0.f), "box2", glm::vec3(0.f,0.f,0.f), ColliderType::BOX, DynamicType::WithPhysics));
	cell.Insert(box1);
	cell.Insert(box2);
	REQUIRE(cell.dynamicColliders.size() == 2);
	cell.Remove(box1);
	REQUIRE(cell.dynamicColliders.size() == 1);
	REQUIRE(cell.GetDynamicColliders().size() == 1);
}