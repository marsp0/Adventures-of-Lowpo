#include "AABB.hpp"
#include <iostream>

AABB::AABB(glm::vec3 center, glm::vec3 axisRadii, bool dynamic, ColliderType colliderType) : 
                        Collider(center, dynamic, colliderType), axisRadii(axisRadii)
{

}

bool AABB::CheckCollision(std::shared_ptr<Triangle> triangle)
{
    // general idea
    // 1. test 9 axis resulting from the cross products of the edges of the AABB and the edges of the triangle
    //      - the edges of the AABB correspond with the directions of the world coordinates.
    // 2. test the 3 normals of the AABB
    // 3. test the triangle normal

    // p0,p1,p2 - the points projected on the current axis.
    // r - "radius" of the AABB on the current axis. The AABB center is on the origin of the axis.
    float projectedPoint0, projectedPoint1, projectedPoint2;
    float radius;
    float min;
    float max;

    // Moving triangle as conceptually moving the AABB to the origin
    glm::vec3 trianglePoint0 = triangle->points[0] - this->center;
    glm::vec3 trianglePoint1 = triangle->points[1] - this->center;
    glm::vec3 trianglePoint2 = triangle->points[2] - this->center;
    std::vector<glm::vec3> trianglePoints{trianglePoint0,trianglePoint1,trianglePoint2};

    // Triangle Edge vectors
    glm::vec3 triangleVector0 = trianglePoint1 - trianglePoint0;
    glm::vec3 triangleVector1 = trianglePoint2 - trianglePoint1;
    glm::vec3 triangleVector2 = trianglePoint0 - trianglePoint2;
    std::vector<glm::vec3> triangleVectors{triangleVector0, triangleVector1, triangleVector2};
    std::vector<glm::vec3> worldVectors{glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,1.f,0.f), glm::vec3(0.f,0.f,1.f)};

    // AABB radius projected onto vector n
    // r = halfwidth.x * abs(dot(i,n)) + halfwidth.y * abs(dot(j,n)) + halfwidth.z * abs(dot(k,n))
    
    for (int i = 0; i < worldVectors.size(); i++)
    {
        for (int j = 0; j < triangleVectors.size(); j++)
        {
            glm::vec3 projectionAxis = glm::cross(worldVectors[i],triangleVectors[j]);
            radius = this->axisRadii.x * abs(glm::dot(worldVectors[0],projectionAxis)) + \
                     this->axisRadii.y * abs(glm::dot(worldVectors[1],projectionAxis)) + \
                     this->axisRadii.z * abs(glm::dot(worldVectors[2],projectionAxis));
            projectedPoint0 = glm::dot(triangleVector0,projectionAxis);
            projectedPoint1 = glm::dot(triangleVector1,projectionAxis);
            projectedPoint2 = glm::dot(triangleVector2,projectionAxis);
            std::vector<float> temp{projectedPoint0, projectedPoint1, projectedPoint2};
            min = projectedPoint0;
            max = projectedPoint0;
            for (int k = 0; k < 3; k++)
            {
                if (temp[k] < min)
                    min = temp[k];
                if (temp[k] > max)
                    max = temp[k];
            }
            if (min > radius || max < -radius)
                return false;
        }
    }
    // test aabb normals
    float minX = trianglePoint0.x;
    float maxX = trianglePoint0.x;

    float minY = trianglePoint0.y;
    float maxY = trianglePoint0.y;

    float minZ = trianglePoint0.z;
    float maxZ = trianglePoint0.z;

    for (int i = 0; i < trianglePoints.size(); i++)
    {
        glm::vec3 current = trianglePoints[i];
        if (current.x < minX)
            minX = current.x;
        if (current.x > maxX)
            maxX = current.x;

        if (current.y < minY)
            minY = current.y;
        if (current.y > maxY)
            maxY = current.y;

        if (current.z < minZ)
            minZ = current.z;
        if (current.z > maxZ)
            maxZ = current.z;
    }

    if (maxX < -this->axisRadii.x || minX > this->axisRadii.x)
        return false;
    if (maxY < -this->axisRadii.y || minY > this->axisRadii.y)
        return false;
    if (maxZ < -this->axisRadii.z || minZ > this->axisRadii.z)
        return false;

    // test triangle normal
    glm::vec3 triangleNormal = glm::cross(triangleVector0, triangleVector1);
    float normal = glm::dot(triangleNormal,trianglePoint0);
    radius = this->axisRadii.x * abs(triangleNormal.x) + this->axisRadii.y * abs(triangleNormal.y) + this->axisRadii.z * abs(triangleNormal.z);
    float s = glm::dot(triangleNormal,this->center) - glm::dot(triangleNormal,trianglePoint0);

    std::cout << "DSdsadsadsad21321321321321321" << std::endl;
    return abs(s) <= radius;
}

bool AABB::CheckCollision(std::shared_ptr<Sphere> sphere)
{

}

bool AABB::CheckCollision(std::shared_ptr<AABB> box)
{


}

bool AABB::CheckCollision(std::shared_ptr<Collider> collider)
{
    // TODO : FIX THIS AS WE HAVE REPEATING FUNCTION IN ALL 3 Colliders
    if (collider->type == ColliderType::BOX)
    {
        std::shared_ptr<AABB> ptr = std::static_pointer_cast<AABB>(collider);
        return this->CheckCollision(ptr);
    }
    else if (collider->type == ColliderType::TRIANGLE)
    {
        std::shared_ptr<Triangle> ptr = std::static_pointer_cast<Triangle>(collider);
        return this->CheckCollision(ptr);
    }
    else
    {
        std::shared_ptr<Sphere> ptr = std::static_pointer_cast<Sphere>(collider);
        return this->CheckCollision(ptr);
    }
}