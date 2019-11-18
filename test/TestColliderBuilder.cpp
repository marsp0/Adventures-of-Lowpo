#include "catch.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"
#include "../src/util.hpp"
#include <chrono>
#include <iostream>

TEST_CASE("Test Collider Builder - normal box")
{
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(0.f, 0.f, 0.f)); // 0
	points.push_back(glm::vec3(2.f, 0.f, 0.f)); // 1
	points.push_back(glm::vec3(0.f, 2.f, 0.f)); // 2
	points.push_back(glm::vec3(2.f, 2.f, 0.f)); // 3
	points.push_back(glm::vec3(0.f, 0.f, 2.f)); // 4
	points.push_back(glm::vec3(2.f, 0.f, 2.f)); // 5
	points.push_back(glm::vec3(2.f, 2.f, 2.f)); // 6
	points.push_back(glm::vec3(0.f, 2.f, 2.f)); // 7

	std::vector<std::pair<glm::vec3, glm::vec3>> expectedEdges;
	expectedEdges.push_back(std::make_pair(points[0], points[4]));
	expectedEdges.push_back(std::make_pair(points[0], points[2]));
	expectedEdges.push_back(std::make_pair(points[0], points[1]));
	expectedEdges.push_back(std::make_pair(points[1], points[5]));
	expectedEdges.push_back(std::make_pair(points[1], points[3]));
	expectedEdges.push_back(std::make_pair(points[4], points[7]));
	expectedEdges.push_back(std::make_pair(points[4], points[5]));
	expectedEdges.push_back(std::make_pair(points[3], points[6]));
	expectedEdges.push_back(std::make_pair(points[3], points[2]));
	expectedEdges.push_back(std::make_pair(points[6], points[7]));
	expectedEdges.push_back(std::make_pair(points[5], points[6]));
	expectedEdges.push_back(std::make_pair(points[2], points[7]));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, 1.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));

	std::vector<std::pair<glm::vec3, glm::vec3>> finalFaces;
	std::vector<std::pair<glm::vec3, glm::vec3>> finalEdges;
	glm::vec3 center;
	ColliderBuilder::Build(points, finalFaces, finalEdges, center);
	// Face check
	REQUIRE(finalFaces.size() == 6);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].first);
			if (dotResult - 1.f < 0.0005 && dotResult > 0.f)
			{
				test = true;
			}
		}
		REQUIRE(test == true);
	}

	// Edges check
	REQUIRE(finalEdges.size() == 12);
	for (int i = 0; i < expectedEdges.size(); i++)
	{
		REQUIRE(ColliderBuilder::ContainsEdge(finalEdges, expectedEdges[i]) == true);
	}
	
}

TEST_CASE("Test Collider Builder - Deformed Box")
{
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(0.f, 0.f, 0.f)); // 0
	points.push_back(glm::vec3(2.f, 0.f, 0.f)); // 1
	points.push_back(glm::vec3(0.f, 1.f, 0.f)); // 2
	points.push_back(glm::vec3(2.f, 2.f, 0.f)); // 3
	points.push_back(glm::vec3(0.f, 0.f, 1.f)); // 4
	points.push_back(glm::vec3(2.f, 0.f, 2.f)); // 5
	points.push_back(glm::vec3(2.f, 2.f, 2.f)); // 6
	points.push_back(glm::vec3(0.f, 1.f, 1.f)); // 7

	std::vector<std::pair<glm::vec3, glm::vec3>> expectedEdges;
	expectedEdges.push_back(std::make_pair(points[0], points[4]));
	expectedEdges.push_back(std::make_pair(points[0], points[2]));
	expectedEdges.push_back(std::make_pair(points[0], points[1]));
	expectedEdges.push_back(std::make_pair(points[1], points[5]));
	expectedEdges.push_back(std::make_pair(points[1], points[3]));
	expectedEdges.push_back(std::make_pair(points[4], points[7]));
	expectedEdges.push_back(std::make_pair(points[4], points[5]));
	expectedEdges.push_back(std::make_pair(points[3], points[6]));
	expectedEdges.push_back(std::make_pair(points[3], points[2]));
	expectedEdges.push_back(std::make_pair(points[6], points[7]));
	expectedEdges.push_back(std::make_pair(points[5], points[6]));
	expectedEdges.push_back(std::make_pair(points[2], points[7]));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::cross(points[3] - points[6], points[7]- points[6]));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::cross(points[7]- points[6], points[5] - points[6]));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));

	std::vector<std::pair<glm::vec3, glm::vec3>> finalFaces;
	std::vector<std::pair<glm::vec3, glm::vec3>> finalEdges;
	glm::vec3 center;
	ColliderBuilder::Build(points, finalFaces, finalEdges, center);

	// Face check
	REQUIRE(finalFaces.size() == 6);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].first);
			if (dotResult - 1.f < 0.0005 && dotResult > 0.f)
			{
				test = true;
			}
		}
		REQUIRE(test == true);
	}

	// Edges check
	REQUIRE(finalEdges.size() == 12);
	for (int i = 0; i < expectedEdges.size(); i++)
	{
		REQUIRE(ColliderBuilder::ContainsEdge(finalEdges, expectedEdges[i]) == true);
	}
}

TEST_CASE("Create box with negative coordinates.")
{
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(0.f, -2.f, 0.f)); // 0
	points.push_back(glm::vec3(2.f, -2.f, 0.f)); // 1
	points.push_back(glm::vec3(0.f, 2.f, 0.f)); // 2
	points.push_back(glm::vec3(2.f, 2.f, 0.f)); // 3
	points.push_back(glm::vec3(0.f, -2.f, 2.f)); // 4
	points.push_back(glm::vec3(2.f, -2.f, 2.f)); // 5
	points.push_back(glm::vec3(2.f, 2.f, 2.f)); // 6
	points.push_back(glm::vec3(0.f, 2.f, 2.f)); // 7

	std::vector<std::pair<glm::vec3, glm::vec3>> expectedEdges;
	expectedEdges.push_back(std::make_pair(points[0], points[4]));
	expectedEdges.push_back(std::make_pair(points[0], points[2]));
	expectedEdges.push_back(std::make_pair(points[0], points[1]));
	expectedEdges.push_back(std::make_pair(points[1], points[5]));
	expectedEdges.push_back(std::make_pair(points[1], points[3]));
	expectedEdges.push_back(std::make_pair(points[4], points[7]));
	expectedEdges.push_back(std::make_pair(points[4], points[5]));
	expectedEdges.push_back(std::make_pair(points[3], points[6]));
	expectedEdges.push_back(std::make_pair(points[3], points[2]));
	expectedEdges.push_back(std::make_pair(points[6], points[7]));
	expectedEdges.push_back(std::make_pair(points[5], points[6]));
	expectedEdges.push_back(std::make_pair(points[2], points[7]));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, 1.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));

	std::vector<std::pair<glm::vec3, glm::vec3>> finalFaces;
	std::vector<std::pair<glm::vec3, glm::vec3>> finalEdges;
	glm::vec3 center;
	ColliderBuilder::Build(points, finalFaces, finalEdges, center);
	// Face check
	REQUIRE(finalFaces.size() == 6);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].first);
			if (dotResult - 1.f < 0.0005 && dotResult > 0.f)
			{
				test = true;
			}
		}
		REQUIRE(test == true);
	}

	// Edges check
	REQUIRE(finalEdges.size() == 12);
	for (int i = 0; i < expectedEdges.size(); i++)
	{
		REQUIRE(ColliderBuilder::ContainsEdge(finalEdges, expectedEdges[i]) == true);
	}
}


TEST_CASE("box with 7 faces and 13 edges")
{
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(0.f, 0.f, 0.f)); // 0 A
	points.push_back(glm::vec3(2.f, 0.f, 0.f)); // 1 B
	points.push_back(glm::vec3(0.f, 1.f, 0.f)); // 2 C
	points.push_back(glm::vec3(2.f, 2.f, 0.f)); // 3 D
	points.push_back(glm::vec3(0.f, 0.f, 0.7f)); // 4 E
	points.push_back(glm::vec3(2.f, 0.f, 2.f)); // 5 F
	points.push_back(glm::vec3(2.f, 2.f, 2.f)); // 6 G
	points.push_back(glm::vec3(0.f, 1.f, 1.f)); // 7 H

	std::vector<std::pair<glm::vec3, glm::vec3>> expectedEdges;
	expectedEdges.push_back(std::make_pair(points[0], points[4]));
	expectedEdges.push_back(std::make_pair(points[0], points[2]));
	expectedEdges.push_back(std::make_pair(points[0], points[1]));
	expectedEdges.push_back(std::make_pair(points[1], points[5]));
	expectedEdges.push_back(std::make_pair(points[1], points[3]));
	expectedEdges.push_back(std::make_pair(points[4], points[7]));
	expectedEdges.push_back(std::make_pair(points[4], points[5]));
	expectedEdges.push_back(std::make_pair(points[3], points[6]));
	expectedEdges.push_back(std::make_pair(points[3], points[2]));
	expectedEdges.push_back(std::make_pair(points[6], points[7]));
	expectedEdges.push_back(std::make_pair(points[5], points[6]));
	expectedEdges.push_back(std::make_pair(points[2], points[7]));
	expectedEdges.push_back(std::make_pair(points[5], points[7]));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::cross(points[3] - points[6], points[7]- points[6]));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::cross(points[7]- points[6], points[5] - points[6]));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));
	expectedNormals.push_back(glm::cross(points[5]- points[4], points[7] - points[4]));

	std::vector<std::pair<glm::vec3, glm::vec3>> finalFaces;
	std::vector<std::pair<glm::vec3, glm::vec3>> finalEdges;
	glm::vec3 center;
	ColliderBuilder::Build(points, finalFaces, finalEdges, center);

	// Face check
	REQUIRE(finalFaces.size() == 7);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].first);
			if (dotResult - 1.f < 0.0005 && dotResult > 0.f)
			{
				test = true;
			}
		}
		REQUIRE(test == true);
	}

	// Edges check
	REQUIRE(finalEdges.size() == 13);
	for (int i = 0; i < expectedEdges.size(); i++)
	{
		REQUIRE(ColliderBuilder::ContainsEdge(finalEdges, expectedEdges[i]) == true);
	}
}
