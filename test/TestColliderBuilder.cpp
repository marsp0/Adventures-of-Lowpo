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

	std::vector<std::pair<int, int>> expectedEdges;
	expectedEdges.push_back(std::make_pair(0, 4));
	expectedEdges.push_back(std::make_pair(0, 2));
	expectedEdges.push_back(std::make_pair(0, 1));
	expectedEdges.push_back(std::make_pair(1, 5));
	expectedEdges.push_back(std::make_pair(1, 3));
	expectedEdges.push_back(std::make_pair(4, 7));
	expectedEdges.push_back(std::make_pair(4, 5));
	expectedEdges.push_back(std::make_pair(3, 6));
	expectedEdges.push_back(std::make_pair(3, 2));
	expectedEdges.push_back(std::make_pair(6, 7));
	expectedEdges.push_back(std::make_pair(5, 6));
	expectedEdges.push_back(std::make_pair(2, 7));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, 1.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));

	std::shared_ptr<Collider> collider = ColliderBuilder::Build(1, DynamicType::Static, points);

	const std::vector<std::pair<int, int>>& finalEdges = collider->GetEdges();
	const std::vector<ColliderFace>& finalFaces = collider->GetFaces();

	// Face check
	REQUIRE(finalFaces.size() == 6);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].normal);
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

	std::vector<std::pair<int, int>> expectedEdges;
	expectedEdges.push_back(std::make_pair(0, 4));
	expectedEdges.push_back(std::make_pair(0, 2));
	expectedEdges.push_back(std::make_pair(0, 1));
	expectedEdges.push_back(std::make_pair(1, 5));
	expectedEdges.push_back(std::make_pair(1, 3));
	expectedEdges.push_back(std::make_pair(4, 7));
	expectedEdges.push_back(std::make_pair(4, 5));
	expectedEdges.push_back(std::make_pair(3, 6));
	expectedEdges.push_back(std::make_pair(3, 2));
	expectedEdges.push_back(std::make_pair(6, 7));
	expectedEdges.push_back(std::make_pair(5, 6));
	expectedEdges.push_back(std::make_pair(2, 7));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::cross(points[3] - points[6], points[7]- points[6]));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::cross(points[7]- points[6], points[5] - points[6]));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));

	std::shared_ptr<Collider> collider = ColliderBuilder::Build(1, DynamicType::Static, points);

	const std::vector<std::pair<int, int>>& finalEdges = collider->GetEdges();
	const std::vector<ColliderFace>& finalFaces = collider->GetFaces();

	// Face check
	REQUIRE(finalFaces.size() == 6);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].normal);
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

	std::vector<std::pair<int, int>> expectedEdges;
	expectedEdges.push_back(std::make_pair(0, 4));
	expectedEdges.push_back(std::make_pair(0, 2));
	expectedEdges.push_back(std::make_pair(0, 1));
	expectedEdges.push_back(std::make_pair(1, 5));
	expectedEdges.push_back(std::make_pair(1, 3));
	expectedEdges.push_back(std::make_pair(4, 7));
	expectedEdges.push_back(std::make_pair(4, 5));
	expectedEdges.push_back(std::make_pair(3, 6));
	expectedEdges.push_back(std::make_pair(3, 2));
	expectedEdges.push_back(std::make_pair(6, 7));
	expectedEdges.push_back(std::make_pair(5, 6));
	expectedEdges.push_back(std::make_pair(2, 7));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, 1.f));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));

	std::shared_ptr<Collider> collider = ColliderBuilder::Build(1, DynamicType::Static, points);

	const std::vector<std::pair<int, int>>& finalEdges = collider->GetEdges();
	const std::vector<ColliderFace>& finalFaces = collider->GetFaces();
	
	// Face check
	REQUIRE(finalFaces.size() == 6);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].normal);
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

	std::vector<std::pair<int, int>> expectedEdges;
	expectedEdges.push_back(std::make_pair(0, 4));
	expectedEdges.push_back(std::make_pair(0, 2));
	expectedEdges.push_back(std::make_pair(0, 1));
	expectedEdges.push_back(std::make_pair(1, 5));
	expectedEdges.push_back(std::make_pair(1, 3));
	expectedEdges.push_back(std::make_pair(4, 7));
	expectedEdges.push_back(std::make_pair(4, 5));
	expectedEdges.push_back(std::make_pair(3, 6));
	expectedEdges.push_back(std::make_pair(3, 2));
	expectedEdges.push_back(std::make_pair(6, 7));
	expectedEdges.push_back(std::make_pair(5, 6));
	expectedEdges.push_back(std::make_pair(2, 7));
	expectedEdges.push_back(std::make_pair(5, 7));

	std::vector<glm::vec3> expectedNormals;
	expectedNormals.push_back(glm::vec3(1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::vec3(-1.f, 0.f, 0.f));
	expectedNormals.push_back(glm::cross(points[3] - points[6], points[7]- points[6]));
	expectedNormals.push_back(glm::vec3(0.f, -1.f, 0.f));
	expectedNormals.push_back(glm::cross(points[7]- points[6], points[5] - points[6]));
	expectedNormals.push_back(glm::vec3(0.f, 0.f, -1.f));
	expectedNormals.push_back(glm::cross(points[5]- points[4], points[7] - points[4]));

	std::shared_ptr<Collider> collider = ColliderBuilder::Build(1, DynamicType::Static, points);

	const std::vector<std::pair<int, int>>& finalEdges = collider->GetEdges();
	const std::vector<ColliderFace>& finalFaces = collider->GetFaces();

	// Face check
	REQUIRE(finalFaces.size() == 7);
	for (int i = 0; i < expectedNormals.size(); i++)
	{
		bool test = false;
		for (int j = 0; j < finalFaces.size(); j++)
		{
			float dotResult = glm::dot(expectedNormals[i], finalFaces[j].normal);
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

TEST_CASE("Face points test")
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

	std::shared_ptr<Collider> collider = ColliderBuilder::Build(1, DynamicType::Static, points);

	std::vector<std::vector<int>> expectedFacePoints;
	expectedFacePoints.push_back(std::vector<int>{0, 2, 4, 7});
	expectedFacePoints.push_back(std::vector<int>{1, 3, 5, 6});
	expectedFacePoints.push_back(std::vector<int>{0, 1, 4, 5});
	expectedFacePoints.push_back(std::vector<int>{2, 3, 6, 7});
	expectedFacePoints.push_back(std::vector<int>{0, 1, 2, 3});
	expectedFacePoints.push_back(std::vector<int>{4, 5, 6, 7});

	const std::vector<ColliderFace>& faces = collider->GetFaces();

	REQUIRE(expectedFacePoints.size() == faces.size());
	for (int i = 0; i < expectedFacePoints.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < faces.size(); j++)
		{
			int count = 0;
			for (int x = 0; x < faces[j].points.size(); x++)
			{
				for (int k = 0; k < expectedFacePoints[i].size(); k++)
				{
					if (faces[j].points[x] == expectedFacePoints[i][k])
						count += 1;
				}
			}
			if (count == 4)
				found = true;
		}
		REQUIRE(found == true);
	}
}