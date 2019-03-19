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
    std::vector<glm::vec3> pointsA = box->GetPoints();
    std::vector<glm::vec3> pointsB = triangle->GetPoints();
    return this->FindDistance(pointsA, pointsB);
}

std::shared_ptr<Collision> CollisionDetector::TriangleToTriangle(std::shared_ptr<Triangle> first, std::shared_ptr<Triangle> second)
{

}


// std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
bool CollisionDetector::TriangleToAABB(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
{
    return this->AABBToTriangle(box, triangle);
}

bool CollisionDetector::FindDistance(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB)
{
    glm::vec3 direction = glm::vec3(1.f,1.f,1.f);
    glm::vec3 a;
    glm::vec3 b = this->GetSupportPoint(pointsA, direction) - this->GetSupportPoint(pointsB, -direction);
    glm::vec3 c;
    glm::vec3 d;
    int simplexSize = 1;
    direction = -b;
    // TODO : add max_number_of_iterations or something to keep
    // the loop from going infinity war.
    while(true)
    {
        a = this->GetSupportPoint(pointsA, direction) - this->GetSupportPoint(pointsB, -direction);
        simplexSize++;
        std::cout << simplexSize << std::endl;
        if (glm::dot(direction,a) <= 0)
        {
            std::cout << "WE ARE NOT INTERSECTING" << std::endl;
            return false;
        }
        if (this->DoSimplex(a,b,c,d,direction,simplexSize))
        {
            std::cout << "EPA CALL" << std::endl;
            glm::vec3 minumTranslationVector = this->ExpandingPolytope(a,b,c,d,pointsA,pointsB);
            std::cout << "MTV is" << std::endl;
            std::cout << minumTranslationVector.x << std::endl;
            std::cout << minumTranslationVector.y << std::endl;
            std::cout << minumTranslationVector.z << std::endl;
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

glm::vec3 CollisionDetector::ExpandingPolytope(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB)
{
    float EPA_TOLERANCE = 0.0001;
    int EPA_MAX_NUM_FACES = 64;
    int EPA_MAX_NUM_LOOSE_EDGES = 32;
    int EPA_MAX_NUM_ITERATIONS = 64;

    glm::vec3 faces[EPA_MAX_NUM_FACES][4];
    glm::vec3 loose_edges[EPA_MAX_NUM_LOOSE_EDGES][2];

    int num_faces = 4;
    int num_loose_edges;

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

        glm::vec3 direction = faces[index][3];
        glm::vec3 newPoint = this->GetSupportPoint(pointsA, direction) - this->GetSupportPoint(pointsB, -direction);

        float temp = glm::dot(direction, newPoint) - minDot;
        if ( temp < EPA_TOLERANCE)
        {
            return direction * glm::dot(newPoint, direction);
        }

        for (int k = 0; k < num_faces; k++)
        {
            // 6. if the current face "sees" the point. dot(face.normal, support.point - face.vertex[0]) > 0
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