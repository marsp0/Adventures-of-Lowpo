#include <iostream>
#include "catch.hpp"
#include "glm/gtc/epsilon.hpp"
#include <string>
#include "../src/Systems/Physics/Plane.hpp"

TEST_CASE("Test Plane")
{
	std::vector<glm::vec3> planePoints{glm::vec3(-1.0, 0.f, -1.f), glm::vec3(1.0, 0.f, -1.f), glm::vec3(1.0f, 0.f, 1.f), glm::vec3(-1.f, 0.f, 1.f)};
	Plane plane(1, glm::vec3(0.0f, 0.0f, 0.f), ColliderType::PLANE, DynamicType::Static, planePoints);

	plane.Update(glm::vec3(1.f, 0.f, 0.f));
	REQUIRE(plane.center.x == 1.f);
	REQUIRE(plane.center.y == 0.f);
	REQUIRE(plane.center.z == 0.f);

	const std::vector<glm::vec3> points = plane.GetPoints();

	REQUIRE(points.size() == 4);

	REQUIRE(points[0].x == 0.f);
	REQUIRE(points[0].y == 0.f);
	REQUIRE(points[0].z == -1.f);

	REQUIRE(points[1].x == 2.f);
	REQUIRE(points[1].y == 0.f);
	REQUIRE(points[1].z == -1.f);

	REQUIRE(points[2].x == 2.f);
	REQUIRE(points[2].y == 0.f);
	REQUIRE(points[2].z == 1.f);

	REQUIRE(points[3].x == 0.f);
	REQUIRE(points[3].y == 0.f);
	REQUIRE(points[3].z == 1.f);

	const std::vector<std::pair<glm::vec3, float>> faces = plane.GetFaces();

	REQUIRE(faces.size() == 1);

	REQUIRE(faces[0].second == 0);
	glm::vec3 e1 = points[1] - points[0];
	glm::vec3 e2 = points[2] - points[0];
	glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
	REQUIRE(faces[0].first.x == normal.x);
	REQUIRE(faces[0].first.y == normal.y);
	REQUIRE(faces[0].first.z == normal.z);

	const std::vector<std::pair<glm::vec3, glm::vec3>> edges = plane.GetEdges();

	REQUIRE(edges.size() == 4);

}