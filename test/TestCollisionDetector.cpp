#include "catch.hpp"
#include <memory>
#include <iostream>
#include "../src/Systems/Physics/Collider.hpp"
#include "../src/Systems/Physics/CollisionDetector.hpp"
#include "../src/Systems/Physics/ColliderBuilder.hpp"
#include "../src/util.hpp"

TEST_CASE("CollisionDetector Test")
{
	CollisionDetector detector = CollisionDetector();

	/*
		types of collisions - Currently we are only able to work with BOXES
		face - vertex
		face - edge
		face - face
		edge - edge
		big face - small face
		small face - big face
	*/

	std::vector<std::pair<glm::vec3, glm::vec3>> edges1;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces1;
	glm::vec3 center1;
	std::vector<glm::vec3> points1;
	points1.push_back(glm::vec3(1.f, 0.f, 0.f));
	points1.push_back(glm::vec3(1.f, 1.5f, 0.f));
	points1.push_back(glm::vec3(2.5f, 0.f, 0.f));
	points1.push_back(glm::vec3(2.5f, 1.5f, 0.f));

	points1.push_back(glm::vec3(1.f, 0.f, 1.5f));
	points1.push_back(glm::vec3(1.f, 1.5f, 1.5f));
	points1.push_back(glm::vec3(2.5f, 0.f, 1.5f));
	points1.push_back(glm::vec3(2.5f, 1.5f, 1.5f));
	ColliderBuilder::Build(points1, faces1, edges1, center1);
	std::shared_ptr<Collider> collider1 = std::make_shared<Collider>(1, center1, points1, edges1, faces1, DynamicType::Static);
		

	std::vector<std::pair<glm::vec3, glm::vec3>> edges2;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces2;
	glm::vec3 center2;
	std::vector<glm::vec3> points2;
	points2.push_back(glm::vec3(0.f, 0.f, 0.f));
	points2.push_back(glm::vec3(2.f, 0.f, 0.f));
	points2.push_back(glm::vec3(2.f, 2.f, 0.f));
	points2.push_back(glm::vec3(0.f, 2.f, 0.f));

	points2.push_back(glm::vec3(0.f, 0.f, 2.f));
	points2.push_back(glm::vec3(2.f, 0.f, 2.f));
	points2.push_back(glm::vec3(2.f, 2.f, 2.f));
	points2.push_back(glm::vec3(0.f, 2.f, 2.f));
	
	ColliderBuilder::Build(points2, faces2, edges2, center2);
	std::shared_ptr<Collider> collider2 = std::make_shared<Collider>(2, center2, points2, edges2, faces2, DynamicType::Dynamic);
	

	std::vector<std::pair<glm::vec3, glm::vec3>> edges5;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces5;
	glm::vec3 center5;
	std::vector<glm::vec3> points5;
	points5.push_back(glm::vec3(-1.9f, 0.1f, 0.1f));
	points5.push_back(glm::vec3(0.1f, 0.1f, 0.1f));
	points5.push_back(glm::vec3(-1.9f, 2.1f, 0.1f));
	points5.push_back(glm::vec3(0.1f, 2.1f, 0.1f));

	points5.push_back(glm::vec3(-1.9f, 0.1f, 2.1f));
	points5.push_back(glm::vec3(0.1f, 0.1f, 2.1f));
	points5.push_back(glm::vec3(-1.9f, 2.1f, 2.1f));
	points5.push_back(glm::vec3(0.1f, 2.1f, 2.1f));
	
	ColliderBuilder::Build(points5, faces5, edges5, center5);
	std::shared_ptr<Collider> collider5 = std::make_shared<Collider>(5, center5, points5, edges5, faces5, DynamicType::Dynamic);

	std::vector<std::pair<glm::vec3, glm::vec3>> edges6;
	std::vector<std::pair<glm::vec3, glm::vec3>> faces6;
	glm::vec3 center6;
	std::vector<glm::vec3> points6;
	points6.push_back(glm::vec3(0.f, 0.f, 0.f));
	points6.push_back(glm::vec3(2.5f, 0.f, 0.f));
	points6.push_back(glm::vec3(2.5f, 2.f, 0.f));
	points6.push_back(glm::vec3(0.f, 2.f, 0.f));

	points6.push_back(glm::vec3(0.f, 0.f, 2.f));
	points6.push_back(glm::vec3(2.5f, 0.f, 2.f));
	points6.push_back(glm::vec3(2.5f, 2.f, 2.f));
	points6.push_back(glm::vec3(0.f, 2.f, 2.f));
	ColliderBuilder::Build(points6, faces6, edges6, center6);

	std::shared_ptr<Collider> collider6 = std::make_shared<Collider>(6, center6, points6, edges6, faces6, DynamicType::Dynamic);
	
}