#include "catch.hpp"
#include <vector>
#include "../src/Systems/Physics/Triangle.hpp"

extern float EPSILON;

TEST_CASE("Triangle Test")
{
	int entityID = 2;
	std::string name = "triangle 1";
	glm::vec3 normal = glm::vec3(0.f,0.f,1.f);
	std::vector<glm::vec3> tempPoints;
	tempPoints.push_back(glm::vec3(0.f,0.f,0.f));
	tempPoints.push_back(glm::vec3(4.f,.2f,0.f));
	tempPoints.push_back(glm::vec3(2.f,4.f,0.f));
	glm::vec3 center = glm::vec3(2.f,2.f,0.f);
	ColliderType colliderType = ColliderType::TRIANGLE;
	DynamicType dynamicType = DynamicType::Static;
	Triangle triangle(entityID, center, name, normal, tempPoints, colliderType, dynamicType);
	triangle.ComputeDerivedData();
	REQUIRE(triangle.entityID == 2);

	REQUIRE(triangle.center.x - 2.f <= EPSILON);
	REQUIRE(triangle.center.y - 2.f <= EPSILON);
	REQUIRE(triangle.center.z - 0.f <= EPSILON);

	REQUIRE(triangle.normal.x - 0.f <= EPSILON);
	REQUIRE(triangle.normal.y - 0.f <= EPSILON);
	REQUIRE(triangle.normal.z - 1.f <= EPSILON);

	const std::vector<glm::vec3> points = triangle.GetPoints();

	REQUIRE(points[0].x - 0.f <= EPSILON);
	REQUIRE(points[0].y - 0.f <= EPSILON);
	REQUIRE(points[0].z - 0.f <= EPSILON);

	REQUIRE(points[1].x - 4.f <= EPSILON);
	REQUIRE(points[1].y - 2.f <= EPSILON);
	REQUIRE(points[1].z - 0.f <= EPSILON);

	REQUIRE(points[2].x - 2.f <= EPSILON);
	REQUIRE(points[2].y - 4.f <= EPSILON);
	REQUIRE(points[2].z - 0.f <= EPSILON);

	REQUIRE(triangle.colliderType == ColliderType::TRIANGLE);
	REQUIRE(triangle.dynamicType == DynamicType::Static);


	const std::vector<std::pair<glm::vec3, glm::vec3>> edges = triangle.GetEdges();

	REQUIRE(edges[0].first.x - 0.f <= EPSILON);
	REQUIRE(edges[0].first.y - 0.f <= EPSILON);
	REQUIRE(edges[0].first.z - 0.f <= EPSILON);
	REQUIRE(edges[0].second.x - 4.f <= EPSILON);
	REQUIRE(edges[0].second.y - 2.f <= EPSILON);
	REQUIRE(edges[0].second.z - 0.f <= EPSILON);

	REQUIRE(edges[1].first.x - 0.f <= EPSILON);
	REQUIRE(edges[1].first.y - 0.f <= EPSILON);
	REQUIRE(edges[1].first.z - 0.f <= EPSILON);
	REQUIRE(edges[1].second.x - 2.f <= EPSILON);
	REQUIRE(edges[1].second.y - 4.f <= EPSILON);
	REQUIRE(edges[1].second.z - 0.f <= EPSILON);

	REQUIRE(edges[2].first.x - 4.f <= EPSILON);
	REQUIRE(edges[2].first.y - 2.f <= EPSILON);
	REQUIRE(edges[2].first.z - 0.f <= EPSILON);
	REQUIRE(edges[2].second.x - 2.f <= EPSILON);
	REQUIRE(edges[2].second.y - 4.f <= EPSILON);
	REQUIRE(edges[2].second.z - 0.f <= EPSILON);
}