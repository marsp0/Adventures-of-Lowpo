#include "catch.hpp"

#include "../src/Systems/Physics/Cell.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"

TEST_CASE("Test Cell")
{
	Cell cell(glm::vec3(10.f,10.f,10.f), 5.f, 1, 1);
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(-1.f, -0.1f, 0.05f));
	points1.push_back(glm::vec3(4.f, -0.1f, 0.05f));
	points1.push_back(glm::vec3(-1.f, 4.f, 0.05f));
	points1.push_back(glm::vec3(4.f, 4.f, 0.05f));

	points1.push_back(glm::vec3(-1.f, -0.1f, -0.2f));
	points1.push_back(glm::vec3(4.f, -0.1f, -0.2f));
	points1.push_back(glm::vec3(-1.f, 4.f, -0.2f));
	points1.push_back(glm::vec3(4.f, 4.f, -0.2f));
	std::shared_ptr<Collider> collider1 = ColliderBuilder::Build(1, DynamicType::Static, points1);

	std::vector<glm::vec3> points3;
	points3.push_back(glm::vec3(3.f, 1.f, 1.f));
	points3.push_back(glm::vec3(0.9f, -1.f, 1.f));
	points3.push_back(glm::vec3(2.f, -2.f, 1.f));
	points3.push_back(glm::vec3(4.f, 0.f, 1.f));

	points3.push_back(glm::vec3(3.f, 1.f, 0.f));
	points3.push_back(glm::vec3(0.9f, -1.f, 0.f));
	points3.push_back(glm::vec3(2.f, -2.f, 0.f));
	points3.push_back(glm::vec3(4.f, 0.f, 0.f));
	std::shared_ptr<Collider> collider3 = ColliderBuilder::Build(3, DynamicType::Dynamic, points3);

	std::vector<glm::vec3> points4;
	points4.push_back(glm::vec3(0.f, 0.f, 0.f));
	points4.push_back(glm::vec3(2.f, 0.f, 0.f));
	points4.push_back(glm::vec3(2.f, 2.f, 0.f));
	points4.push_back(glm::vec3(0.f, 2.f, 0.f));

	points4.push_back(glm::vec3(0.f, 0.f, 2.f));
	points4.push_back(glm::vec3(2.f, 0.f, 2.f));
	points4.push_back(glm::vec3(2.f, 2.f, 2.f));
	points4.push_back(glm::vec3(0.f, 2.f, 2.f));
	std::shared_ptr<Collider> collider4 = ColliderBuilder::Build(4, DynamicType::Dynamic, points4);

	cell.Insert(collider1);
	cell.Insert(collider3);
	cell.Insert(collider4);
	REQUIRE(cell.GetDynamicColliders().size() == 2);
	REQUIRE(cell.GetStaticColliders().size() == 1);
	cell.Remove(collider3);
	REQUIRE(cell.GetStaticColliders().size() == 1);
	REQUIRE(cell.GetDynamicColliders().size() == 1);
}