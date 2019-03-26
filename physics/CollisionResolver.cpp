#include "CollisionResolver.hpp"

CollisionResolver::CollisionResolver()
{

}

void CollisionResolver::GenerateOrthonormalBasis(glm::vec3& x, glm::vec3& y, glm::vec3& z)
{
    glm::vec3 worldUp = glm::vec3(0.f,1.f,0.f);

    if (glm::dot(x, worldUp) > 0.f)
    {
        worldUp = glm::vec3(1.f, 0.f, 0.f);
    }
    
    z = glm::normalize(glm::cross(x, worldUp));
    y = glm::normalize(glm::cross(z, x));
}