#include <glm/gtc/epsilon.hpp>
#include <iostream>
#include <unordered_set>
#include "ColliderBuilder.hpp"
#include "../../util.hpp"

const float epsilon = 0.005f;

std::shared_ptr<Collider> ColliderBuilder::Build(int id, DynamicType colliderType, std::vector<glm::vec3> points)
{
	/**
	TODO : Good as a starting point, but this is very slow. Optimize! 
	*/
	std::vector<ColliderFace> finalFaces;
	std::vector<std::pair<int, int>> finalEdges;
	glm::vec3 center;
	std::vector<std::unique_ptr<cFace>> faces;
	center = ColliderBuilder::GetCenter(points);
	ColliderBuilder::FindExtremeFaces(faces, points, center);
	ColliderBuilder::MergeFaces(faces, finalFaces, finalEdges, points);
	return std::make_shared<Collider>(id, center, points, finalEdges, finalFaces, colliderType);
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
				std::unique_ptr<cFace> face = ColliderBuilder::CreateFace(i, j, k, center, points);
				face->points.insert(i);
				face->points.insert(j);
				face->points.insert(k);
				bool isExtreme = true;
				float planeOffset = glm::dot(face->normal, points[i]);
				
				for (int x = 0; x < points.size(); x++)
				{
					if (x == i || x == j || x == k)
						continue;
					glm::vec3 pointOnFace = points[face->edges[0].first];
					glm::vec3 pointToFace = points[x] - pointOnFace;

					float pointFaceDot = glm::dot(face->normal, points[x]);
					
					if (glm::dot(pointToFace, face->normal) > 0.f)
						isExtreme = false;
					else if (pointFaceDot - planeOffset < epsilon && pointFaceDot - planeOffset >= 0.f)
						face->points.insert(x);
				}
				if (isExtreme)
					faces.push_back(std::move(face));
			}
		}
	}
}

void ColliderBuilder::MergeFaces(	std::vector<std::unique_ptr<cFace>>& faces,
									std::vector<ColliderFace>& finalFaces,
									std::vector<std::pair<int, int>>& finalEdges,
									std::vector<glm::vec3>& points)
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
 		std::vector<int> 								currentVertices{faces[i]->edges[0].first, faces[i]->edges[1].first, faces[i]->edges[2].first};
 		std::vector<std::pair<int, int>> 				currentEdges;
 		std::unordered_set<int> 						pointsOnFace;	

 		for (int j = i + 1; j < faces.size(); j++)
		{
			if (glm::all(glm::epsilonEqual(faces[i]->normal, faces[j]->normal, epsilon)))
			{
				currentFaces.push_back(faces[j].get());
				currFaceIndices.push_back(j);
				currentVertices.push_back(faces[j]->edges[0].first);
				currentVertices.push_back(faces[j]->edges[1].first);
				currentVertices.push_back(faces[j]->edges[2].first);
				for (std::unordered_set<int>::iterator x = faces[i]->points.begin(); x != faces[i]->points.end(); x++)
				{
					pointsOnFace.insert(*x);
				}
			}
		}
 		
 		// create the actual face

 		for (int k = 0; k < currentFaces.size(); k++)
 		{
 			for (int x = 0; x < currentFaces[k]->edges.size(); x++)
 			{
 				bool isExtreme = ColliderBuilder::IsExtremeEdge(currentVertices, currentFaces[k]->edges[x], points);
 				if (isExtreme && !ColliderBuilder::ContainsEdge(currentEdges, currentFaces[k]->edges[x]))
 				{
 					currentEdges.push_back(currentFaces[k]->edges[x]);
 				}
 			}
 		}

 		ColliderFace finalFace;
 		finalFace.normal = currentFaces[0]->normal;
 		for (std::unordered_set<int>::iterator k = pointsOnFace.begin(); k != pointsOnFace.end(); k++)
 		{
 			finalFace.points.push_back(*k);
 		}
 		finalFaces.push_back(finalFace);

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

bool ColliderBuilder::IsExtremeEdge(std::vector<int> pointIndices, std::pair<int, int> edge, std::vector<glm::vec3>& points)
{
	/*				 / Q
			 	   / |
			     /   |
			   /\    |
			M ================= N
					 P
	*/
	int index = 0;
	glm::vec3 edgeFirst = points[edge.first];
	glm::vec3 edgeSecond = points[edge.second];
	for (int i = 0; i < pointIndices.size(); ++i)
	{
		if (pointIndices[i] != edge.first && pointIndices[i] != edge.second)
		{
			index = i;
			break;
		}
	}
	glm::vec3 indexPoint = points[pointIndices[index]];
	glm::vec3 MQ = indexPoint - edgeFirst;
	glm::vec3 MN = glm::normalize(edgeSecond - edgeFirst);
	glm::vec3 PQ = glm::normalize(MQ - glm::dot(MQ, MN) * MN);
	for (int i = 0; i < pointIndices.size(); i++)
	{
		glm::vec3 currPoint = points[pointIndices[i]];
		glm::vec3 currDir = glm::normalize(currPoint - edgeFirst);
		if (glm::dot(currDir, PQ) < 0.f)
		{
			return false;
		}
	}
	return true;
}

bool ColliderBuilder::ContainsEdge(std::vector<std::pair<int, int>> edges, std::pair<int, int> edge)
{
	for (int i = 0; i < edges.size(); i++)
	{
		bool first = edges[i].first == edge.first && edges[i].second == edge.second;
		bool second = edges[i].first == edge.second && edges[i].second == edge.first;
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

std::unique_ptr<cFace> ColliderBuilder::CreateFace(	int v1, int v2, int v3, glm::vec3 center, std::vector<glm::vec3>& points)
{
	glm::vec3 normal = glm::normalize(glm::cross(points[v2]-points[v1], points[v3]-points[v1]));
	if (glm::dot(points[v1]-center, normal) < 0.f)
		normal = -normal;

	std::unique_ptr<cFace> face = std::make_unique<cFace>();
	face->normal = normal;
	face->edges.push_back(std::make_pair(v1, v2));
	face->edges.push_back(std::make_pair(v2, v3));
	face->edges.push_back(std::make_pair(v3, v1));
	return face;
}