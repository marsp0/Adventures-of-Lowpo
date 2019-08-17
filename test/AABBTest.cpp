#include <iostream>
#include "catch.hpp"
#include "glm/gtc/epsilon.hpp"
#include <string>
#include "../src/Systems/Physics/AABB.hpp"

extern float EPSILON;

TEST_CASE("Test AABB")
{
	int id = 1;
	glm::vec3 center = glm::vec3(2.f,2.f,2.f);
	std::string name = "box1";
	glm::vec3 axisRadii = glm::vec3(1.f,2.f,3.f);
	ColliderType colliderType = ColliderType::BOX;
	DynamicType dynamicType = DynamicType::Dynamic;
	AABB box = AABB(id, center, name, axisRadii, colliderType, dynamicType);
	box.ComputeDerivedData();

	SECTION("Test Init")
	{
		REQUIRE(box.entityID == 1);
		REQUIRE(box.name == "box1");
		REQUIRE(box.center.x - 2.f <= EPSILON);
		REQUIRE(box.center.y - 2.f <= EPSILON);
		REQUIRE(box.center.z - 2.f <= EPSILON);

		REQUIRE(box.axisRadii.x - 1.f <= EPSILON);
		REQUIRE(box.axisRadii.y - 2.f <= EPSILON);
		REQUIRE(box.axisRadii.z - 3.f <= EPSILON);

		REQUIRE(box.colliderType == ColliderType::BOX);
		REQUIRE(box.dynamicType == DynamicType::Dynamic);

		// POINTS

		const std::vector<glm::vec3> points = box.GetPoints();

		REQUIRE(points[0].x - 1.f <= EPSILON);
		REQUIRE(points[0].y - 0.f <= EPSILON);
		REQUIRE(points[0].z + 1.f <= EPSILON);

		REQUIRE(points[1].x - 3.f <= EPSILON);
		REQUIRE(points[1].y - 0.f <= EPSILON);
		REQUIRE(points[1].z + 1.f <= EPSILON);

		REQUIRE(points[2].x - 3.f <= EPSILON);
		REQUIRE(points[2].y - 0.f <= EPSILON);
		REQUIRE(points[2].z - 5.f <= EPSILON);

		REQUIRE(points[3].x - 1.f <= EPSILON);
		REQUIRE(points[3].y - 0.f <= EPSILON);
		REQUIRE(points[3].z - 5.f <= EPSILON);

		REQUIRE(points[4].x - 1.f <= EPSILON);
		REQUIRE(points[4].y - 4.f <= EPSILON);
		REQUIRE(points[4].z + 1.f <= EPSILON);

		REQUIRE(points[5].x - 3.f <= EPSILON);
		REQUIRE(points[5].y - 4.f <= EPSILON);
		REQUIRE(points[5].z + 1.f <= EPSILON);

		REQUIRE(points[6].x - 3.f <= EPSILON);
		REQUIRE(points[6].y - 4.f <= EPSILON);
		REQUIRE(points[6].z - 5.f <= EPSILON);

		REQUIRE(points[7].x - 1.f <= EPSILON);
		REQUIRE(points[7].y - 4.f <= EPSILON);
		REQUIRE(points[7].z - 5.f <= EPSILON);

		const std::vector<std::pair<glm::vec3, glm::vec3>> edges = box.GetEdges();

		REQUIRE(edges[0].first.x - 1.f <= EPSILON);
		REQUIRE(edges[0].first.y - 0.f <= EPSILON);
		REQUIRE(edges[0].first.z + 1.f <= EPSILON);
		REQUIRE(edges[0].second.x - 3.f <= EPSILON);
		REQUIRE(edges[0].second.y - 0.f <= EPSILON);
		REQUIRE(edges[0].second.z + 1.f <= EPSILON);

		REQUIRE(edges[1].first.x - 1.f <= EPSILON);
		REQUIRE(edges[1].first.y - 0.f <= EPSILON);
		REQUIRE(edges[1].first.z + 1.f <= EPSILON);
		REQUIRE(edges[1].second.x - 1.f <= EPSILON);
		REQUIRE(edges[1].second.y - 0.f <= EPSILON);
		REQUIRE(edges[1].second.z - 5.f <= EPSILON);

		REQUIRE(edges[2].first.x - 1.f <= EPSILON);
		REQUIRE(edges[2].first.y - 0.f <= EPSILON);
		REQUIRE(edges[2].first.z + 1.f <= EPSILON);
		REQUIRE(edges[2].second.x - 1.f <= EPSILON);
		REQUIRE(edges[2].second.y - 4.f <= EPSILON);
		REQUIRE(edges[2].second.z + 1.f <= EPSILON);

		REQUIRE(edges[3].first.x - 3.f <= EPSILON);
		REQUIRE(edges[3].first.y - 0.f <= EPSILON);
		REQUIRE(edges[3].first.z + 1.f <= EPSILON);
		REQUIRE(edges[3].second.x - 3.f <= EPSILON);
		REQUIRE(edges[3].second.y - 0.f <= EPSILON);
		REQUIRE(edges[3].second.z - 5.f <= EPSILON);

		REQUIRE(edges[4].first.x - 3.f <= EPSILON);
		REQUIRE(edges[4].first.y - 0.f <= EPSILON);
		REQUIRE(edges[4].first.z + 1.f <= EPSILON);
		REQUIRE(edges[4].second.x - 3.f <= EPSILON);
		REQUIRE(edges[4].second.y - 4.f <= EPSILON);
		REQUIRE(edges[4].second.z + 1.f <= EPSILON);

		REQUIRE(edges[5].first.x - 3.f <= EPSILON);
		REQUIRE(edges[5].first.y - 0.f <= EPSILON);
		REQUIRE(edges[5].first.z - 5.f <= EPSILON);
		REQUIRE(edges[5].second.x - 1.f <= EPSILON);
		REQUIRE(edges[5].second.y - 0.f <= EPSILON);
		REQUIRE(edges[5].second.z - 5.f <= EPSILON);

		REQUIRE(edges[6].first.x - 3.f <= EPSILON);
		REQUIRE(edges[6].first.y - 0.f <= EPSILON);
		REQUIRE(edges[6].first.z - 5.f <= EPSILON);
		REQUIRE(edges[6].second.x - 3.f <= EPSILON);
		REQUIRE(edges[6].second.y - 4.f <= EPSILON);
		REQUIRE(edges[6].second.z - 5.f <= EPSILON);

		REQUIRE(edges[7].first.x - 1.f <= EPSILON);
		REQUIRE(edges[7].first.y - 0.f <= EPSILON);
		REQUIRE(edges[7].first.z - 5.f <= EPSILON);
		REQUIRE(edges[7].second.x - 1.f <= EPSILON);
		REQUIRE(edges[7].second.y - 4.f <= EPSILON);
		REQUIRE(edges[7].second.z - 5.f <= EPSILON);

		REQUIRE(edges[8].first.x - 1.f <= EPSILON);
		REQUIRE(edges[8].first.y - 4.f <= EPSILON);
		REQUIRE(edges[8].first.z + 1.f <= EPSILON);
		REQUIRE(edges[8].second.x - 3.f <= EPSILON);
		REQUIRE(edges[8].second.y - 4.f <= EPSILON);
		REQUIRE(edges[8].second.z + 1.f <= EPSILON);

		REQUIRE(edges[9].first.x - 1.f <= EPSILON);
		REQUIRE(edges[9].first.y - 4.f <= EPSILON);
		REQUIRE(edges[9].first.z + 1.f <= EPSILON);
		REQUIRE(edges[9].second.x - 1.f <= EPSILON);
		REQUIRE(edges[9].second.y - 4.f <= EPSILON);
		REQUIRE(edges[9].second.z - 5.f <= EPSILON);

		REQUIRE(edges[10].first.x - 3.f <= EPSILON);
		REQUIRE(edges[10].first.y - 4.f <= EPSILON);
		REQUIRE(edges[10].first.z + 1.f <= EPSILON);
		REQUIRE(edges[10].second.x - 3.f <= EPSILON);
		REQUIRE(edges[10].second.y - 4.f <= EPSILON);
		REQUIRE(edges[10].second.z - 5.f <= EPSILON);

		REQUIRE(edges[11].first.x - 3.f <= EPSILON);
		REQUIRE(edges[11].first.y - 4.f <= EPSILON);
		REQUIRE(edges[11].first.z - 5.f <= EPSILON);
		REQUIRE(edges[11].second.x - 1.f <= EPSILON);
		REQUIRE(edges[11].second.y - 4.f <= EPSILON);
		REQUIRE(edges[11].second.z - 5.f <= EPSILON);

	}
	SECTION("Testing Update Method")
	{
		box.Update(glm::vec3(1.f,1.f,1.f));

		const std::vector<glm::vec3> points = box.GetPoints();

		REQUIRE(points[0].x - 2.f <= EPSILON);
		REQUIRE(points[0].y - 1.f <= EPSILON);
		REQUIRE(points[0].z + 0.f <= EPSILON);

		REQUIRE(points[1].x - 4.f <= EPSILON);
		REQUIRE(points[1].y - 1.f <= EPSILON);
		REQUIRE(points[1].z + 0.f <= EPSILON);

		REQUIRE(points[2].x - 4.f <= EPSILON);
		REQUIRE(points[2].y - 1.f <= EPSILON);
		REQUIRE(points[2].z - 6.f <= EPSILON);

		REQUIRE(points[3].x - 2.f <= EPSILON);
		REQUIRE(points[3].y - 1.f <= EPSILON);
		REQUIRE(points[3].z - 6.f <= EPSILON);

		REQUIRE(points[4].x - 2.f <= EPSILON);
		REQUIRE(points[4].y - 5.f <= EPSILON);
		REQUIRE(points[4].z + 0.f <= EPSILON);

		REQUIRE(points[5].x - 4.f <= EPSILON);
		REQUIRE(points[5].y - 5.f <= EPSILON);
		REQUIRE(points[5].z + 0.f <= EPSILON);

		REQUIRE(points[6].x - 4.f <= EPSILON);
		REQUIRE(points[6].y - 5.f <= EPSILON);
		REQUIRE(points[6].z - 6.f <= EPSILON);

		REQUIRE(points[7].x - 2.f <= EPSILON);
		REQUIRE(points[7].y - 5.f <= EPSILON);
		REQUIRE(points[7].z - 6.f <= EPSILON);

		const std::vector<std::pair<glm::vec3, glm::vec3>> edges = box.GetEdges();

		REQUIRE(edges[0].first.x - 2.f <= EPSILON);
		REQUIRE(edges[0].first.y - 1.f <= EPSILON);
		REQUIRE(edges[0].first.z + 0.f <= EPSILON);
		REQUIRE(edges[0].second.x - 4.f <= EPSILON);
		REQUIRE(edges[0].second.y - 1.f <= EPSILON);
		REQUIRE(edges[0].second.z + 0.f <= EPSILON);

		REQUIRE(edges[1].first.x - 2.f <= EPSILON);
		REQUIRE(edges[1].first.y - 1.f <= EPSILON);
		REQUIRE(edges[1].first.z + 0.f <= EPSILON);
		REQUIRE(edges[1].second.x - 2.f <= EPSILON);
		REQUIRE(edges[1].second.y - 1.f <= EPSILON);
		REQUIRE(edges[1].second.z - 6.f <= EPSILON);

		REQUIRE(edges[2].first.x - 2.f <= EPSILON);
		REQUIRE(edges[2].first.y - 1.f <= EPSILON);
		REQUIRE(edges[2].first.z + 0.f <= EPSILON);
		REQUIRE(edges[2].second.x - 2.f <= EPSILON);
		REQUIRE(edges[2].second.y - 5.f <= EPSILON);
		REQUIRE(edges[2].second.z + 0.f <= EPSILON);

		REQUIRE(edges[3].first.x - 4.f <= EPSILON);
		REQUIRE(edges[3].first.y - 1.f <= EPSILON);
		REQUIRE(edges[3].first.z + 0.f <= EPSILON);
		REQUIRE(edges[3].second.x - 4.f <= EPSILON);
		REQUIRE(edges[3].second.y - 1.f <= EPSILON);
		REQUIRE(edges[3].second.z - 6.f <= EPSILON);

		REQUIRE(edges[4].first.x - 4.f <= EPSILON);
		REQUIRE(edges[4].first.y - 1.f <= EPSILON);
		REQUIRE(edges[4].first.z + 0.f <= EPSILON);
		REQUIRE(edges[4].second.x - 4.f <= EPSILON);
		REQUIRE(edges[4].second.y - 5.f <= EPSILON);
		REQUIRE(edges[4].second.z + 0.f <= EPSILON);

		REQUIRE(edges[5].first.x - 4.f <= EPSILON);
		REQUIRE(edges[5].first.y - 1.f <= EPSILON);
		REQUIRE(edges[5].first.z - 6.f <= EPSILON);
		REQUIRE(edges[5].second.x - 2.f <= EPSILON);
		REQUIRE(edges[5].second.y - 1.f <= EPSILON);
		REQUIRE(edges[5].second.z - 6.f <= EPSILON);

		REQUIRE(edges[6].first.x - 4.f <= EPSILON);
		REQUIRE(edges[6].first.y - 1.f <= EPSILON);
		REQUIRE(edges[6].first.z - 6.f <= EPSILON);
		REQUIRE(edges[6].second.x - 4.f <= EPSILON);
		REQUIRE(edges[6].second.y - 5.f <= EPSILON);
		REQUIRE(edges[6].second.z - 6.f <= EPSILON);

		REQUIRE(edges[7].first.x - 2.f <= EPSILON);
		REQUIRE(edges[7].first.y - 1.f <= EPSILON);
		REQUIRE(edges[7].first.z - 6.f <= EPSILON);
		REQUIRE(edges[7].second.x - 2.f <= EPSILON);
		REQUIRE(edges[7].second.y - 5.f <= EPSILON);
		REQUIRE(edges[7].second.z - 6.f <= EPSILON);

		REQUIRE(edges[8].first.x - 2.f <= EPSILON);
		REQUIRE(edges[8].first.y - 5.f <= EPSILON);
		REQUIRE(edges[8].first.z + 0.f <= EPSILON);
		REQUIRE(edges[8].second.x - 4.f <= EPSILON);
		REQUIRE(edges[8].second.y - 5.f <= EPSILON);
		REQUIRE(edges[8].second.z + 0.f <= EPSILON);

		REQUIRE(edges[9].first.x - 2.f <= EPSILON);
		REQUIRE(edges[9].first.y - 5.f <= EPSILON);
		REQUIRE(edges[9].first.z + 0.f <= EPSILON);
		REQUIRE(edges[9].second.x - 2.f <= EPSILON);
		REQUIRE(edges[9].second.y - 5.f <= EPSILON);
		REQUIRE(edges[9].second.z - 6.f <= EPSILON);

		REQUIRE(edges[10].first.x - 4.f <= EPSILON);
		REQUIRE(edges[10].first.y - 5.f <= EPSILON);
		REQUIRE(edges[10].first.z + 0.f <= EPSILON);
		REQUIRE(edges[10].second.x - 4.f <= EPSILON);
		REQUIRE(edges[10].second.y - 5.f <= EPSILON);
		REQUIRE(edges[10].second.z - 6.f <= EPSILON);

		REQUIRE(edges[11].first.x - 4.f <= EPSILON);
		REQUIRE(edges[11].first.y - 5.f <= EPSILON);
		REQUIRE(edges[11].first.z - 6.f <= EPSILON);
		REQUIRE(edges[11].second.x - 2.f <= EPSILON);
		REQUIRE(edges[11].second.y - 5.f <= EPSILON);
		REQUIRE(edges[11].second.z - 6.f <= EPSILON);
	}

}