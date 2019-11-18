#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

struct cFace
{
	glm::vec3 normal;
	std::vector<std::pair<glm::vec3, glm::vec3>> edges;
};

/**
ColliderBuilder is used to generate the edges and faces of a collider from a set of points.
The algorithm used is a naive one and should be optimized at a later stage.
*/
class ColliderBuilder
{
	public:

		static void Build(std::vector<glm::vec3> points,
							std::vector<std::pair<glm::vec3, glm::vec3>>& finalFaces,
							std::vector<std::pair<glm::vec3, glm::vec3>>& finalEdges,
							glm::vec3& center);
		static void FindExtremeFaces(	std::vector<std::unique_ptr<cFace>>& faces, 
										std::vector<glm::vec3>& points,
										glm::vec3 center);
		static bool IsExtremeEdge(std::vector<glm::vec3> points, std::pair<glm::vec3, glm::vec3> edge);
		static glm::vec3 GetCenter(std::vector<glm::vec3> points);
		static std::unique_ptr<cFace> CreateFace(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 center);
		static void MergeFaces(std::vector<std::unique_ptr<cFace>>& faces,
								std::vector<std::pair<glm::vec3, glm::vec3>>& finalFaces,
								std::vector<std::pair<glm::vec3, glm::vec3>>& finalEdges);
		static bool ContainsEdge(std::vector<std::pair<glm::vec3, glm::vec3>> edges, std::pair<glm::vec3, glm::vec3> edge);
};