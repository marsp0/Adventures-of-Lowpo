#include "Plane.hpp"

Plane::Plane(int entityID,
			 glm::vec3 center,
			 ColliderType colliderType,
			 DynamicType dynamicType,
			 std::vector<glm::vec3> points) : \
			 Collider(entityID, center, colliderType, dynamicType)
{
	this->points = points;
}

Plane::~Plane()
{

}

void Plane::Update(glm::vec3 translation)
{
	this->center += translation;
	for (int i = 0; i < this->points.size(); i++)
	{
		this->points[i] += translation;
	}
	this->ComputeDerivedData();
}

void Plane::ComputeDerivedData()
{
	this->pointsOnFaces.clear();
	this->pointsOnFaces.push_back(this->points[0]);

	this->faces.clear();
	glm::vec3 e1 = this->points[1] - this->points[0];
	glm::vec3 e2 = this->points[2] - this->points[0];
	glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
	float distance = glm::dot(this->points[0], normal);
	this->faces.push_back(std::make_pair(normal, distance));

	this->edges.clear();
	// TODO : here we are at the mercy of the exporter. This needs to get cleaned and 
	// and I have to make sure that the resulting edges are not diagonals.
	this->edges.push_back(std::make_pair(this->points[0], this->points[1]));
	this->edges.push_back(std::make_pair(this->points[1], this->points[2]));
	this->edges.push_back(std::make_pair(this->points[2], this->points[3]));
	this->edges.push_back(std::make_pair(this->points[3], this->points[0]));
}