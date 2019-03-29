#include <iostream>
#include <unordered_map>
#include "CollisionDetector.hpp"


CollisionDetector::CollisionDetector()
{

}

bool CollisionDetector::CheckCollision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
    if (first->colliderType == ColliderType::BOX && second->colliderType == ColliderType::TRIANGLE || first->colliderType == ColliderType::TRIANGLE && second->colliderType == ColliderType::BOX)
    {
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
    else if (first->colliderType == ColliderType::BOX && second->colliderType == ColliderType::BOX)
    {
        
    }
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
    
}

// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle)
bool CollisionDetector::AABBToTriangle(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle)
{
    // Strategy
    // 1. check the axes of the AABB (world axes)
    // 2. Check the cross products between the world axes and the edges of the triangle

    std::vector<glm::vec3> pointsA = box->GetPoints();
    std::vector<glm::vec3> pointsB = triangle->GetPoints();

    std::vector<glm::vec3> separatingAxis;
    glm::vec3 x = glm::vec3(1.f,0.f,0.f);
    glm::vec3 y = glm::vec3(0.f,1.f,0.f);
    glm::vec3 z = glm::vec3(0.f,0.f,1.f);
    glm::vec3 a = pointsB[0];
    glm::vec3 b = pointsB[1];
    glm::vec3 c = pointsB[2];
    glm::vec3 triangleNormal = glm::cross(b-a, c-a);

    separatingAxis.push_back(x);
    separatingAxis.push_back(y);
    separatingAxis.push_back(z);

    glm::vec3 ab = glm::cross(glm::cross(b - a, triangleNormal), b - a);
    separatingAxis.push_back(glm::cross(x, ab));
    separatingAxis.push_back(glm::cross(y, ab));
    separatingAxis.push_back(glm::cross(z ,ab));

    glm::vec3 ac = glm::cross(glm::cross(c - a, triangleNormal), c - a);
    separatingAxis.push_back(glm::cross(x, ac));
    separatingAxis.push_back(glm::cross(y, ac));
    separatingAxis.push_back(glm::cross(z, ac));

    glm::vec3 bc = glm::cross(glm::cross(c - b, triangleNormal), c - b);
    separatingAxis.push_back(glm::cross(x, bc));
    separatingAxis.push_back(glm::cross(y, bc));
    separatingAxis.push_back(glm::cross(z, bc));

    for (int i = 0; i < separatingAxis.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, separatingAxis[i]))
        {
            return false;
        }
    }
    return true;
}

std::shared_ptr<Collision> CollisionDetector::TriangleToTriangle(std::shared_ptr<Triangle> first, std::shared_ptr<Triangle> second)
{

}


// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
bool CollisionDetector::TriangleToAABB(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
{
    return this->AABBToTriangle(box, triangle);
}

bool CollisionDetector::IsSeparatingAxis(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB, glm::vec3 direction)
{
    float minA = glm::dot(direction, this->GetSupportPoint(pointsA, -direction));
    float maxA = glm::dot(direction, this->GetSupportPoint(pointsA, direction));
    float minB = glm::dot(direction, this->GetSupportPoint(pointsB, -direction));
    float maxB = glm::dot(direction, this->GetSupportPoint(pointsB, direction));
    if (minA > maxB || maxA < minB)
    {
        return true;
    }
    return false;
}

bool CollisionDetector::FindDistance(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB)
{
    std::vector<glm::vec3> supportMapA;
    std::vector<glm::vec3> supportMapB;
    std::vector<glm::vec3> supportMap;
    // Setting an initial direction in which search for support points.
    glm::vec3 direction = glm::vec3(1.f,1.f,1.f);
    // a,b,c,d will be the points of the generated simplices
    // a will always be the newest point.
    glm::vec3 a;
    glm::vec3 supportPointA = this->GetSupportPoint(pointsA, direction);
    glm::vec3 supportPointB = this->GetSupportPoint(pointsB, -direction);
    glm::vec3 b = supportPointA - supportPointB;
    supportMapA.push_back(supportPointA);
    supportMapB.push_back(supportPointB);
    supportMap.push_back(b);
    glm::vec3 c;
    glm::vec3 d;
    int simplexSize = 1;
    // setting the next search direction to the opposite of the vector that represents the point that was found above.
    // This means that we want to go in the direction of the origin.
    direction = -b;
    // TODO : add max_number_of_iterations or something to keep
    // the loop from going infinity war.
    while(true)
    {
        glm::vec3 supportPointA = this->GetSupportPoint(pointsA, direction);
        glm::vec3 supportPointB = this->GetSupportPoint(pointsB, -direction);
        // getting the newest support point in the direction of the origin.
        a = supportPointA - supportPointB;
        supportMapA.push_back(supportPointA);
        supportMapB.push_back(supportPointB);
        supportMap.push_back(a);
        simplexSize++;
        // if the dot product between the direction and the found point is less then 0, it means
        // that the point has not crossed the origin and so we dont have intersection.
        //    p\
        //      \    /a
        //       \  /
        //       /\/
        //    d /  \
        // p - plane passing through O
        // d - search direction
        // a - the newly found point.
        if (glm::dot(direction,a) <= 0)
        {
            return false;
        }
        // if they are in the same direction
        // we want to check of the simplex we have is encapsulating the origin
        // in our engine we only care for the tetrahedron as we are in 3D.
        if (this->DoSimplex(a,b,c,d,direction,simplexSize))
        {
            // if we have a tetrahedron encapsulating the origin, then
            // we want to generate :
            // Contact Normal - Done
            // Penetration Depth - Done
            // ContactPointA - IN PROGRESS
            // ContactPointB - IN PROGRESS
            glm::vec3 minumTranslationVector = this->ExpandingPolytope(a,b,c,d,pointsA,pointsB, supportMap, supportMapA, supportMapB);
            // Call generate contact
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

glm::vec3 CollisionDetector::ExpandingPolytope(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB, std::vector<glm::vec3>& supportMap, std::vector<glm::vec3>& supportMapA, std::vector<glm::vec3>& supportMapB)
{
    float EPA_TOLERANCE = 0.0001;
    int EPA_MAX_NUM_FACES = 64;
    int EPA_MAX_NUM_LOOSE_EDGES = 32;
    int EPA_MAX_NUM_ITERATIONS = 64;

    
    // faces will contain all the triangles of the polytop that we are generating.
    // first 3 elements are the points and the last one is the normal to the triangle.
    glm::vec3 faces[EPA_MAX_NUM_FACES][4];
    // 
    glm::vec3 loose_edges[EPA_MAX_NUM_LOOSE_EDGES][2];

    int num_faces = 4;
    int num_loose_edges = 0;

    // Generate the initial tetrahedron from the points we got from GJK.
    // Triangle wounding is important. We are going counterclockwise
    // b
    // |\
    // | \ a
    // | /
    // |/
    // c
    faces[0][0] = a;
    faces[0][1] = b;
    faces[0][2] = c;
    faces[0][3] = glm::normalize(glm::cross(b - a, c - a));

    faces[1][0] = a;
    faces[1][1] = c;
    faces[1][2] = d;
    faces[1][3] = glm::normalize(glm::cross(c - a, d - a));

    faces[2][0] = a;
    faces[2][1] = d;
    faces[2][2] = b;
    faces[2][3] = glm::normalize(glm::cross(d - a, b - a));

    faces[3][0] = b;
    faces[3][1] = d;
    faces[3][2] = c;
    faces[3][3] = glm::normalize(glm::cross(d - b, c - b));

    int index = 0;
    for (int i = 0; i < EPA_MAX_NUM_ITERATIONS; i++)
    {
        // get the face closest to the origin.
        float minDot = glm::dot(faces[0][3], faces[0][0]);
        for (int j = 0; j < num_faces; j++)
        {
            float current = glm::dot(faces[j][3], faces[j][0]);
            if (current < minDot)
            {
                minDot = current;
                index = j;
            }
        }

        // get a point in the direction of the normal to the triangle that is closest to the origin.
        glm::vec3 direction = faces[index][3];
        glm::vec3 supportPointA = this->GetSupportPoint(pointsA, direction);
        glm::vec3 supportPointB = this->GetSupportPoint(pointsB, -direction);
        glm::vec3 newPoint = supportPointA - supportPointB;
        supportMapA.push_back(supportPointA);
        supportMapB.push_back(supportPointB);
        supportMap.push_back(newPoint);

        if ( glm::dot(direction, newPoint) - minDot < EPA_TOLERANCE)
        {
            glm::vec3 projectedPoint = direction * glm::dot(direction, newPoint);
            std::cout << "Projected point" << std::endl;
            std::cout << projectedPoint.x << std::endl;
            std::cout << projectedPoint.y << std::endl;
            std::cout << projectedPoint.z << std::endl;
            std::vector<float> barycentricResult = this->BarycentricCoordinates(faces[index][0], faces[index][1], faces[index][2], projectedPoint);
            float u = barycentricResult[0];
            float v = barycentricResult[1];
            float w = barycentricResult[2];
            glm::vec3 Aa, Ab, Ac;
            glm::vec3 Ba, Bb, Bc;
            for (int i = 0; i < supportMap.size(); i++)
            {
                if (supportMap[i] == faces[index][0])
                {
                    Aa = supportMapA[i];
                    Ba = supportMapB[i];
                }
                else if (supportMap[i] == faces[index][1])
                {
                    Ab = supportMapA[i];
                    Bb = supportMapB[i];
                }
                else if (supportMap[i] == faces[index][2])
                {
                    Ac = supportMapA[i];
                    Bc = supportMapB[i];
                }
            }
            glm::vec3 contactPointA = (u * Aa + v * Ab + w * Ac);
            glm::vec3 contactPointB = (u * Ba + v * Bb + w * Bc);
            std::cout << "Object A - " << pointsA.size() << std::endl;
            std::cout << contactPointA.x << std::endl;
            std::cout << contactPointA.y << std::endl;
            std::cout << contactPointA.z << std::endl;
            std::cout << "-------" << std::endl;
            std::cout << "Object B - " << pointsB.size() << std::endl;
            std::cout << contactPointB.x << std::endl;
            std::cout << contactPointB.y << std::endl;
            std::cout << contactPointB.z << std::endl;
            std::cout << "-------" << std::endl;
            return projectedPoint;
        }

        for (int k = 0; k < num_faces; k++)
        {
            glm::vec3 currentNormal = faces[k][3];
            glm::vec3 currentVertex = faces[k][0];
            if (glm::dot(currentNormal, newPoint - currentVertex) > 0)
            {
                // 7. add edges of triangle to loose edge list. If edges are there, delete them from the array.
                for (int j = 0; j < 3; j++)
                {
                    glm::vec3 currentEdge[2] = {faces[k][j], faces[k][(j + 1) % 3]};
                    bool found = false;
                    for (int t = 0; t < num_loose_edges; t++)
                    {
                        if (loose_edges[t][1] == currentEdge[0] && loose_edges[t][0] == currentEdge[1])
                        {
                            loose_edges[t][0] = loose_edges[num_loose_edges-1][0];
                            loose_edges[t][1] = loose_edges[num_loose_edges-1][1];
                            num_loose_edges--;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        if (num_loose_edges >= EPA_MAX_NUM_LOOSE_EDGES) break;
                        loose_edges[num_loose_edges][0] = currentEdge[0];
                        loose_edges[num_loose_edges][1] = currentEdge[1];
                        num_loose_edges++;
                    }
                }
                faces[k][0] = faces[num_faces-1][0];
                faces[k][1] = faces[num_faces-1][1];
                faces[k][2] = faces[num_faces-1][2];
                faces[k][3] = faces[num_faces-1][3];
                num_faces--;
                k--;
            }
        }

        // Add faces to polytope.
        for (int k =0; k < num_loose_edges; k++)
        {
            if (num_faces >= EPA_MAX_NUM_FACES) break;
            faces[num_faces][0] = loose_edges[k][0];
            faces[num_faces][1] = loose_edges[k][1];
            faces[num_faces][2] = newPoint;
            faces[num_faces][3] = glm::normalize(glm::cross(loose_edges[k][1] - loose_edges[k][0], newPoint - loose_edges[k][0]));

            //Check for wrong normal to maintain CCW winding
            float bias = 0.000001; //in case dot result is only slightly < 0 (because origin is on face)
            if(dot(faces[num_faces][0], faces[num_faces][3]) + bias < 0){
                glm::vec3 temp = faces[num_faces][0];
                faces[num_faces][0] = faces[num_faces][1];
                faces[num_faces][1] = temp;
                faces[num_faces][3] = -faces[num_faces][3];
            }
            num_faces++;
        }
    }
    std::cout << "EPA did not converge" << std::endl;
    return faces[index][3] * glm::dot(faces[index][3], faces[index][0]);
}

std::vector<float> CollisionDetector::BarycentricCoordinates(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 p)
{
    glm::vec3 v0 = b - a;
    glm::vec3 v1 = c - a;
    glm::vec3 v2 = p - a;

    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.f - v - w;

    return std::vector<float>{u,v,w};
}