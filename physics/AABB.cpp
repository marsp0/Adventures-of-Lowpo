#include "AABB.hpp"

AABB::AABB(glm::vec3 center, glm::vec3 axisRadii, bool dynamic) : 
                        Collider(center, dynamic), axisRadii(axisRadii)
{

}

bool AABB::CheckCollision(Triangle triangle)
{

}

bool AABB::CheckCollision(Sphere sphere)
{

}

bool AABB::CheckCollision(AABB box)
{

}