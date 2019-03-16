#include <iostream>

#include "CollisionDetector.hpp"


CollisionDetector::CollisionDetector()
{

}

bool CollisionDetector::CheckCollision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
    if (first->colliderType == ColliderType::BOX && second->colliderType == ColliderType::TRIANGLE || first->colliderType == ColliderType::TRIANGLE && second->colliderType == ColliderType::BOX)
        if (first->colliderType == ColliderType::BOX)
        {
            std::shared_ptr<AABB> box = std::dynamic_pointer_cast<AABB>(first);
            std::shared_ptr<Triangle> triangle = std::dynamic_pointer_cast<Triangle>(second);
            return this->AABBToTriangle(box,triangle);
        }
        std::shared_ptr<AABB> box = std::dynamic_pointer_cast<AABB>(second);
        std::shared_ptr<Triangle> triangle = std::dynamic_pointer_cast<Triangle>(first);
        return this->AABBToTriangle(box,triangle);
        
}

std::shared_ptr<Collision> CollisionDetector::AABBToAABB(std::shared_ptr<AABB> first, std::shared_ptr<AABB> second)
{
    if (first->center.x - first->axisRadii.x > second->center.x + second->axisRadii.x || \
        first->center.x + first->axisRadii.x < second->center.x - second->axisRadii.x)
    {
        return nullptr;
    }
    else if (first->center.y - first->axisRadii.y > second->center.y + second->axisRadii.y || \
        first->center.y + first->axisRadii.y < second->center.y - second->axisRadii.y)
    {
        return nullptr;
    }
    else if (first->center.z - first->axisRadii.z > second->center.z + second->axisRadii.z || \
        first->center.z + first->axisRadii.z < second->center.z - second->axisRadii.z)
    {
        return nullptr;
    }
    // return true;
}

// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle)
bool CollisionDetector::AABBToTriangle(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle)
{
//     // general idea
//     // 1. test 9 axis resulting from the cross products of the edges of the AABB and the edges of the triangle
//     //      - the edges of the AABB correspond with the directions of the world coordinates.
//     // 2. test the 3 normals of the AABB
//     // 3. test the triangle normal

//     // p0,p1,p2 - the points projected on the current axis.
//     // r - "radius" of the AABB on the current axis. The AABB center is on the origin of the axis.
//     float projectedPoint0, projectedPoint1, projectedPoint2;
//     float radius;
//     float min;
//     float max;

//     // Moving triangle as conceptually moving the AABB to the origin
//     glm::vec3 trianglePoint0 = triangle->points[0] - box->center;
//     glm::vec3 trianglePoint1 = triangle->points[1] - box->center;
//     glm::vec3 trianglePoint2 = triangle->points[2] - box->center;
//     std::vector<glm::vec3> trianglePoints{trianglePoint0,trianglePoint1,trianglePoint2};

//     // Triangle Edge vectors
//     glm::vec3 triangleVector0 = trianglePoint1 - trianglePoint0;
//     glm::vec3 triangleVector1 = trianglePoint2 - trianglePoint1;
//     glm::vec3 triangleVector2 = trianglePoint0 - trianglePoint2;
//     std::vector<glm::vec3> triangleVectors{triangleVector0, triangleVector1, triangleVector2};
//     std::vector<glm::vec3> worldVectors{glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,1.f,0.f), glm::vec3(0.f,0.f,1.f)};

//     // AABB radius projected onto vector n
//     // r = halfwidth.x * abs(dot(i,n)) + halfwidth.y * abs(dot(j,n)) + halfwidth.z * abs(dot(k,n))
    
//     for (int i = 0; i < worldVectors.size(); i++)
//     {
//         for (int j = 0; j < triangleVectors.size(); j++)
//         {
//             glm::vec3 projectionAxis = glm::cross(worldVectors[i],triangleVectors[j]);
//             radius = box->axisRadii.x * abs(glm::dot(worldVectors[0],projectionAxis)) + \
//                      box->axisRadii.y * abs(glm::dot(worldVectors[1],projectionAxis)) + \
//                      box->axisRadii.z * abs(glm::dot(worldVectors[2],projectionAxis));
//             projectedPoint0 = glm::dot(triangleVector0,projectionAxis);
//             projectedPoint1 = glm::dot(triangleVector1,projectionAxis);
//             projectedPoint2 = glm::dot(triangleVector2,projectionAxis);
//             std::vector<float> temp{projectedPoint0, projectedPoint1, projectedPoint2};
//             min = projectedPoint0;
//             max = projectedPoint0;
//             for (int k = 0; k < 3; k++)
//             {
//                 if (temp[k] < min)
//                     min = temp[k];
//                 if (temp[k] > max)
//                     max = temp[k];
//             }
//             if (min > radius || max < -radius)
//                 return nullptr;
//         }
//     }
//     // test aabb normals
//     float minX = trianglePoint0.x;
//     float maxX = trianglePoint0.x;

//     float minY = trianglePoint0.y;
//     float maxY = trianglePoint0.y;

//     float minZ = trianglePoint0.z;
//     float maxZ = trianglePoint0.z;

//     for (int i = 0; i < trianglePoints.size(); i++)
//     {
//         glm::vec3 current = trianglePoints[i];
//         if (current.x < minX)
//             minX = current.x;
//         if (current.x > maxX)
//             maxX = current.x;

//         if (current.y < minY)
//             minY = current.y;
//         if (current.y > maxY)
//             maxY = current.y;

//         if (current.z < minZ)
//             minZ = current.z;
//         if (current.z > maxZ)
//             maxZ = current.z;
//     }

//     if (maxX < -box->axisRadii.x || minX > box->axisRadii.x)
//         return nullptr;
//     if (maxY < -box->axisRadii.y || minY > box->axisRadii.y)
//         return nullptr;
//     if (maxZ < -box->axisRadii.z || minZ > box->axisRadii.z)
//         return nullptr;

//     // test triangle normal
//     glm::vec3 triangleNormal = glm::cross(triangleVector0, triangleVector1);
//     float normal = glm::dot(triangleNormal,trianglePoint0);
//     radius = box->axisRadii.x * abs(triangleNormal.x) + box->axisRadii.y * abs(triangleNormal.y) + box->axisRadii.z * abs(triangleNormal.z);
//     float s = glm::dot(triangleNormal,box->center) - glm::dot(triangleNormal,trianglePoint0);

//     return abs(s) <= radius;
    std::vector<glm::vec3> pointsA = box->GetPoints();
    std::vector<glm::vec3> pointsB = triangle->GetPoints();
    return this->FindDistance(pointsA, pointsB);
}

std::shared_ptr<Collision> CollisionDetector::AABBToSphere(std::shared_ptr<AABB> box, std::shared_ptr<Sphere> sphere)
{
    
}

std::shared_ptr<Collision> CollisionDetector::TriangleToTriangle(std::shared_ptr<Triangle> first, std::shared_ptr<Triangle> second)
{

}

std::shared_ptr<Collision> CollisionDetector::TriangleToSphere(std::shared_ptr<Triangle> triangle, std::shared_ptr<Sphere> sphere)
{

}

// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
bool CollisionDetector::TriangleToAABB(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
{
    return this->AABBToTriangle(box, triangle);
}

// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Sphere> first, std::shared_ptr<Sphere> second)
// {

// }

// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Sphere> sphere, std::shared_ptr<AABB> box)
// {

// }

// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Sphere> sphere, std::shared_ptr<Triangle> triangle)
// {

// }

bool CollisionDetector::FindDistance(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB)
{
    glm::vec3 direction = glm::vec3(1.f,1.f,1.f);
    glm::vec3 a;
    glm::vec3 b = this->GetSupportPoint(pointsA, direction) - this->GetSupportPoint(pointsB, -direction);
    glm::vec3 c;
    glm::vec3 d;
    int simplexSize = 1;
    direction = -b;
    while(true)
    {
        a = this->GetSupportPoint(pointsA, direction) - this->GetSupportPoint(pointsB, -direction);
        simplexSize++;
        if (glm::dot(direction,a) <= 0)
        {
            std::cout << "WE ARE NOT INTERSECTING" << std::endl;
            return false;
        }
        if (this->DoSimplex(a,b,c,d,direction,simplexSize))
        {
            std::cout << "INTERSECTION FOUND " << std::endl;
            return true;
        }
    }
    
}

bool CollisionDetector::DoSimplex(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, glm::vec3& direction, int& simplexSize)
{
    if (simplexSize == 2)
    {
        direction = glm::cross(glm::cross(b - a, -a), b - a);
        c = b;
        b = a;
        simplexSize = 2;
        return false;
    }
    else if (simplexSize == 3)
    {
        // Triangle
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        // triangle's normal
        glm::vec3 abc = glm::cross(ab, ac);
        glm::vec3 abPerp = glm::cross(ab, abc);
        
        simplexSize = 2;
        if (glm::dot(abPerp, -a) > 0)
        {
            // in front of the perpendicular of the AB edge
            c = b;
            b = a;
            direction = glm::cross(glm::cross(ab,-a),ab);
            return false;;
        }

        glm::vec3 acPerp = glm::cross(abc, ac);
        if (glm::dot(acPerp, -a) > 0)
        {
            // in front of the perpendicular to the edge AC 
            b = a;
            direction = glm::cross(glm::cross(ac, -a), ac);
            return false;
        }

        simplexSize = 3;
        if (glm::dot(abc, -a) > 0)
        {
            // we are above the abc triangle
            d = c;
            c = b;
            b = a;
            direction = abc;
            return false;
        }
        else
        {
            // we are below the triangle
            d = b;
            b = a;
            direction = -abc;
            return false;
        }
    }
    else
    {
        // Tetrahedron
        // A is at the top pyramid.
        glm::vec3 abc = glm::cross(b-a, c-a);
        glm::vec3 acd = glm::cross(c-a, d-a);
        glm::vec3 adb = glm::cross(d-a, b-a);

        simplexSize = 3;

        if (glm::dot(abc, -a) > 0)
        {
            d = c;
            c = b;
            b = a;
            direction = abc;
            return false;
        }
        else if (glm::dot(acd, -a) > 0)
        {
            b = a;
            direction = acd;
            return false;
        }
        else if (glm::dot(adb, -a) > 0)
        {
            c = d;
            d = b;
            b = a;
            direction = adb;
            return false;
        }
        else 
        {
            // Intersection;
            return true;
        }
    }
}

glm::vec3 CollisionDetector::GetSupportPoint(std::vector<glm::vec3>& points, glm::vec3 direction)
{
    std::cout << points.size() << std::endl;
    assert(points.size() != 0);
    float max = glm::dot(direction, points[0]);
    int index = 0;
    for (int i = 0; i < points.size(); i++)
    {
        float current = glm::dot(direction, points[i]);
        if (current > max)
        {
            max = current;
            index = i;
        }
    }
    return points[index];
}