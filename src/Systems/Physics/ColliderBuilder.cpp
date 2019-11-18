#include <glm/gtc/epsilon.hpp>
#include <iostream>
#include <unordered_set>
#include "ColliderBuilder.hpp"
#include "../../util.hpp"

const float epsilon = 0.005f;

void ColliderBuilder::Build(std::vector<glm::vec3> points,
							std::vector<std::pair<glm::vec3, glm::vec3>>& finalFaces,
							std::vector<std::pair<glm::vec3, glm::vec3>>& finalEdges,
							glm::vec3& center)
{
	/**
	TODO : Good as a starting point, but this is very slow. Optimize! 
	*/

	std::vector<std::unique_ptr<cFace>> faces;
	center = ColliderBuilder::GetCenter(points);
	ColliderBuilder::FindExtremeFaces(faces, points, center);
	ColliderBuilder::MergeFaces(faces, finalFaces, finalEdges);
}

void ColliderBuilder::FindExtremeFaces(	std::vector<std::unique_ptr<cFace>>& faces,
										std::vector<glm::vec3>& points,
										glm::vec3 center)
{
	for (int i = 0; i < points.size(); i++)
	{
		for (int j = i + 1; j < points.size(); j++)
		{
			for (int k = j + 1; k < points.size(); k++)
			{
				std::unique_ptr<cFace> face = ColliderBuilder::CreateFace(points[i],points[j],points[k], center);
				bool isExtreme = true;
				for (int x = 0; x < points.size(); x++)
				{
					if (x == i || x == j || x == k)
						continue;
					
					glm::vec3 pointToFace = points[x] - face->edges[0].first;
					if (glm::dot(pointToFace, face->normal) > 0.f)
					{
						isExtreme = false;
					}
				}
				if (isExtreme)
					faces.push_back(std::move(face));
			}
		}
	}
}

void ColliderBuilder::MergeFaces(std::vector<std::unique_ptr<cFace>>& faces,
								std::vector<std::pair<glm::vec3, glm::vec3>>& finalFaces,
								std::vector<std::pair<glm::vec3, glm::vec3>>& finalEdges)
{
	std::unordered_set<int> visitedKeys;
 	for (int i = 0; i < faces.size(); i++)
 	{
 		// check if the index has already been visited.
 		std::unordered_set<int>::iterator isVisited = visitedKeys.find(i);
 		if (isVisited != visitedKeys.end())
 			continue;

 		// list of faces that are part of the final face
 		std::vector<cFace*> 							currentFaces{faces[i].get()};
 		// indices of faces that should be market visited.
 		std::vector<int> 								currFaceIndices{i};
 		std::vector<glm::vec3> 							currentVertices{faces[i]->edges[0].first, faces[i]->edges[1].first, faces[i]->edges[2].first};
 		std::vector<std::pair<glm::vec3, glm::vec3>> 	currentEdges;

 		for (int j = i + 1; j < faces.size(); j++)
		{
			if (glm::all(glm::epsilonEqual(faces[i]->normal, faces[j]->normal, epsilon)))
			{
				currentFaces.push_back(faces[j].get());
				currFaceIndices.push_back(j);
				currentVertices.push_back(faces[j]->edges[0].first);
				currentVertices.push_back(faces[j]->edges[1].first);
				currentVertices.push_back(faces[j]->edges[2].first);
			}
		}
 		
 		// create the actual face

 		for (int k = 0; k < currentFaces.size(); k++)
 		{
 			for (int x = 0; x < currentFaces[k]->edges.size(); x++)
 			{
 				bool isExtreme = ColliderBuilder::IsExtremeEdge(currentVertices, currentFaces[k]->edges[x]);
 				if (isExtreme && !ColliderBuilder::ContainsEdge(currentEdges, currentFaces[k]->edges[x]))
 				{
 					currentEdges.push_back(currentFaces[k]->edges[x]);
 				}
 			}
 		}

 		finalFaces.push_back(std::make_pair(currentFaces[0]->normal, currentFaces[0]->edges[0].first));
 		for (int k = 0; k < currentEdges.size(); k++)
 		{
 			if (!ColliderBuilder::ContainsEdge(finalEdges, currentEdges[k]))
 			{
 				finalEdges.push_back(currentEdges[k]);
 			}
 		}

 		// add visited indices.
 		for (int x = 0; x < currFaceIndices.size(); x++)
 		{
 			visitedKeys.insert(currFaceIndices[x]);
 		}
 	}
}

bool ColliderBuilder::IsExtremeEdge(std::vector<glm::vec3> points, std::pair<glm::vec3, glm::vec3> edge)
{
	/*				 / Q
			 	   / |
			     /   |
			   /\    |
			M ================= N
					 P
	*/
	int index = 0;
	for (int i = 0; i < points.size(); ++i)
	{
		if (!glm::all(glm::epsilonEqual(points[i], edge.first, epsilon)) && !glm::all(glm::epsilonEqual(points[i], edge.second, epsilon)))
		{
			index = i;
			break;
		}
	}
	glm::vec3 MQ = points[index] - edge.first;
	glm::vec3 MN = glm::normalize(edge.second - edge.first);
	glm::vec3 PQ = glm::normalize(MQ - glm::dot(MQ, MN) * MN);
	for (int i = 0; i < points.size(); i++)
	{
		glm::vec3 currDir = glm::normalize(points[i] - edge.first);
		if (glm::dot(currDir, PQ) < 0.f)
		{
			return false;
		}
	}
	return true;
}

bool ColliderBuilder::ContainsEdge(std::vector<std::pair<glm::vec3, glm::vec3>> edges, std::pair<glm::vec3, glm::vec3> edge)
{
	for (int i = 0; i < edges.size(); i++)
	{
		bool first = glm::all(glm::epsilonEqual(edges[i].first, edge.first, epsilon)) && glm::all(glm::epsilonEqual(edges[i].second, edge.second, epsilon));
		bool second = glm::all(glm::epsilonEqual(edges[i].first, edge.second, epsilon)) && glm::all(glm::epsilonEqual(edges[i].second, edge.first, epsilon));
		if ( first || second )
			return true;
	}
	return false;
}

glm::vec3 ColliderBuilder::GetCenter(std::vector<glm::vec3> points)
{
	glm::vec3 center = glm::vec3(0.f,0.f,0.f);
	float count = points.size();
	for (int i=0; i < points.size(); i++)
	{
		center += points[i];
	}
	center /= count;
	return center;
}

std::unique_ptr<cFace> ColliderBuilder::CreateFace(	glm::vec3 v1,
													glm::vec3 v2,
													glm::vec3 v3,
													glm::vec3 center)
{
	glm::vec3 normal = glm::normalize(glm::cross(v2-v1, v3-v1));
	if (glm::dot(v1-center, normal) < 0.f)
		normal = -normal;

	std::unique_ptr<cFace> face = std::make_unique<cFace>();
	face->normal = normal;
	face->edges.push_back(std::make_pair(v1, v2));
	face->edges.push_back(std::make_pair(v2, v3));
	face->edges.push_back(std::make_pair(v3, v1));
	return face;
}