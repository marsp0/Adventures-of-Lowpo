#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Collider.hpp"

struct cFace
{
	glm::vec3 normal;
	std::vector<std::pair<int, int>> edges;
	std::unordered_set<int> points;
};

/**
ColliderBuilder is used to generate the edges and faces of a collider from a set of points.
The algorithm used is a naive one and should be optimized at a later stage.
*/
class ColliderBuilder
{
	public:

		static std::shared_ptr<Collider> Build(int id, DynamicType colliderType, std::vector<glm::vec3> points);
		static void FindExtremeFaces(	std::vector<std::unique_ptr<cFace>>& faces, 
										std::vector<glm::vec3>& points,
										glm::vec3 center);
		static bool IsExtremeEdge(std::vector<int> pointIndices, std::pair<int, int> edge, std::vector<glm::vec3>& points);
		static glm::vec3 GetCenter(std::vector<glm::vec3> points);
		static std::unique_ptr<cFace> CreateFace(int v1, int v2, int v3, glm::vec3 center, std::vector<glm::vec3>& points);
		static void MergeFaces(std::vector<std::unique_ptr<cFace>>& faces,
								std::vector<ColliderFace>& finalFaces,
								std::vector<std::pair<int, int>>& finalEdges,
								std::vector<glm::vec3>& points);
		static bool ContainsEdge(std::vector<std::pair<int, int>> edges, std::pair<int, int> edge);
};