#include "catch.hpp"

#include "../src/Systems/Physics/Cell.hpp"
#include "../src/Systems/Physics/AABB.hpp"
#include "../src/Systems/Physics/Triangle.hpp"

TEST_CASE("Test Cell")
{
	glm::vec3 center = glm::vec3(10.f,10.f,10.f);
	float halfWidth = 5.f;
	int row = 1;
	int col = 1;
	Cell cell(center, halfWidth, row, col);
	std::vector<glm::vec3> points{glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,0.f,0.f)};
	std::shared_ptr<Triangle> triangle1 = std::make_shared<Triangle>(Triangle(2, glm::vec3(0.f,0.f,0.f), "triangle1", glm::vec3(0.f,0.f,0.f), points, ColliderType::TRIANGLE, DynamicType::Static));
	std::shared_ptr<Triangle> triangle2 = std::make_shared<Triangle>(Triangle(2, glm::vec3(0.f,0.f,0.f), "triangle2", glm::vec3(0.f,0.f,0.f), points, ColliderType::TRIANGLE, DynamicType::Static));
	std::shared_ptr<AABB> box1 = std::make_shared<AABB>(AABB(3, glm::vec3(0.f,0.f,0.f), "box1", glm::vec3(0.f,0.f,0.f), ColliderType::BOX, DynamicType::Dynamic));
	std::shared_ptr<AABB> box2 = std::make_shared<AABB>(AABB(3, glm::vec3(0.f,0.f,0.f), "box2", glm::vec3(0.f,0.f,0.f), ColliderType::BOX, DynamicType::WithPhysics));
	cell.Insert(triangle1);
	cell.Insert(triangle2);
	cell.Insert(box1);
	cell.Insert(box2);
	REQUIRE(cell.dynamicColliders.size() == 2);
	REQUIRE(cell.staticColliders.size() == 2);
	cell.Remove(triangle1);
	cell.Remove(box1);
	REQUIRE(cell.dynamicColliders.size() == 1);
	REQUIRE(cell.staticColliders.size() == 1);
	REQUIRE(cell.GetDynamicColliders().size() == 1);
	REQUIRE(cell.GetStaticColliders().size() == 1);
}