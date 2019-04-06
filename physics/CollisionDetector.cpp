#include <iostream>
#include <unordered_map>
#include <memory>
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

    float tempPenetrationDepth = 0.f;
    float penetrationDepthFace = 10000.f;
    float penetrationDepthEdge = 10000.f;
    float minDistanceBetweenEdges = 10000.f;

    glm::vec3 axisFace;
    glm::vec3 axisEdge;

    int indexEdgeA;
    int indexEdgeB;
    bool indexFaceA = false;

    // NEW IMPLEMENTATION
    // get edges and faces
    const std::vector<std::pair<glm::vec3, float>>& facesA = box->GetFaces();
    const std::vector<std::pair<glm::vec3, float>>& facesB = triangle->GetFaces();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesA = box->GetEdges();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesB = triangle->GetEdges();

    // TODO : refactor this into a function
    for (int i = 0; i < facesA.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesA[i].first, tempPenetrationDepth))
        {
            return false;
        }
        if (tempPenetrationDepth < penetrationDepthFace)
        {
            axisFace = facesA[i].first;
            penetrationDepthFace = tempPenetrationDepth;
            indexFaceA = true;
        }
    }

    // TODO : refactor this into a function
    for (int i = 0; i < facesB.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesB[i].first, tempPenetrationDepth))
        {
            return false;
        }
        if (tempPenetrationDepth < penetrationDepthFace)
        {
            axisFace = facesB[i].first;
            penetrationDepthFace = tempPenetrationDepth; 
            indexFaceA = false;
        }
    }

    // TODO : refactor this into a function
    for (int i = 0; i < edgesA.size(); i++)
    {
        for (int j = 0; j < edgesB.size(); j++)
        {
            // take the cross
            glm::vec3 edgeA = edgesA[i].second - edgesA[i].first;
            glm::vec3 edgeB = edgesB[j].second - edgesB[j].first;
            glm::vec3 separatingAxis = glm::cross(edgeA, edgeB);

            // maintain normal orientation
            if (glm::dot(edgesA[i].first - box->center, separatingAxis) < 0)
            {
                separatingAxis = -separatingAxis;
            }
            // SAT check
            if (this->IsSeparatingAxis(pointsA, pointsB, separatingAxis, tempPenetrationDepth) && glm::length(separatingAxis) > 0.f)
            {
                return false;
            }
            // we need this as we can have multiple edges with the same penetration depth,
            // but where the actual distance between the edges is different. AABB one such collider
            float edgeDistance = this->GetShortestDistanceBetweenEdges(edgesA[i], edgesB[j]);
            if (tempPenetrationDepth <= penetrationDepthEdge && edgeDistance < minDistanceBetweenEdges)
            {
                // Add this check to the if above to make sure that we get the correct edge
                axisEdge = separatingAxis;
                penetrationDepthEdge = tempPenetrationDepth;
                minDistanceBetweenEdges = edgeDistance;
                indexEdgeA = i;
                indexEdgeB = j;
            }
        }
    }

    // check if contact is edge edge or face whatever
    if (penetrationDepthEdge < penetrationDepthFace)
    {
        // TODO : Refactor this part as the same chunk is present in GetShortestDistanceBetweenEdges

        // 1. retrieve edges
        std::pair<glm::vec3, glm::vec3> edgeA = edgesA[indexEdgeA];
        std::pair<glm::vec3, glm::vec3> edgeB = edgesB[indexEdgeB];
        // 2. compute shortest distance between the two edges (perp vector to both of the edges)
        // page 146 in Real-Time Collision Detection book
        glm::vec3 d1 = edgeA.second - edgeA.first;
        glm::vec3 d2 = edgeB.second - edgeB.first;
        glm::vec3 r  = edgeA.first - edgeB.first;
        float a = glm::dot(d1, d1);
        float b = glm::dot(d1, d2);
        float c = glm::dot(d1, r);
        float e = glm::dot(d2, d2);
        float f = glm::dot(d2, r);
        float d = a * e - b * b;
        assert(d != 0.f);

        float s = (b * f - c * e) / d;
        float t = (a * f - b * c) / d;

        glm::vec3 l1 = edgeA.first + s * d1;
        glm::vec3 l2 = edgeB.first + t * d2;

        glm::vec3 result = 0.5f * (l2 - l1);
        // 3. return the point that is halfway through the vector in the same direction.
    }
    else
    {
        // face face contact logic here
        // normal is the axis
        if (indexFaceA)
        {
            // clip against the face of the box

            // get 4 distinct planes 
            std::vector<glm::vec3> planeNormals;
            std::vector<glm::vec3> alreadyChecked;
            std::vector<std::pair<glm::vec3, glm::vec3>> planeEdges;
            for (int i = 0; i < edgesA.size(); i++)
            {
                glm::vec3 first = edgesA[i].first;
                glm::vec3 second = edgesA[i].second;
                glm::vec3 edge = second - first;
                glm::vec3 planeNormal = glm::cross(edge, axisFace);
                if (glm::dot(first - box->center, planeNormal) < 0.0f)
                {
                    planeNormal = -planeNormal;
                }
                if (glm::dot(edge, axisFace) == 0.f)
                {
                    bool shouldAdd = true;
                    for (int j = 0; j < planeNormals.size(); j++)
                    {
                        if (planeNormal == planeNormals[j])
                        {
                            shouldAdd = false;
                            break;
                        }
                    }
                    if (shouldAdd)
                    {
                        
                        planeNormals.push_back(planeNormal);
                        planeEdges.push_back(edgesA[i]);
                    }
                }
            }
            // clip against all
            std::vector<glm::vec3> result;
            for (int i = 0 ; i < planeEdges.size(); i++)
            {
                glm::vec3 first = planeEdges[i].first;
                glm::vec3 second = planeEdges[i].second;
                glm::vec3 currentEdge = second - first;
                glm::vec3 planeNormal = planeNormals[i];

                float d =   -planeNormal.x * first.x - \
                            -planeNormal.y * first.y - \
                            -planeNormal.z * first.z;
                
                std::vector<glm::vec3> temp = this->Clip(edgesB, std::make_pair(planeNormal, d));
                result.insert(result.end(), temp.begin(), temp.end());
            }
            std::cout << result.size() <<std::endl;
            // clip the result against (result is a vector of points and not edges. DOt product to determine
            // if the points are below or above wont work)
        }
        else
        {
            // clip against the face of the triangle

            // construct the planes surrounding the triangle.
            // we can use the edges and their normals to construct the planes.

            // Clip against 3 planes
            // get points "inside" of those 3 planes
            
            std::vector<glm::vec3> result;
            for (int i = 0; i < edgesB.size(); i++)
            {
                std::pair<glm::vec3, glm::vec3> currentEdge = edgesB[i];
                glm::vec3 currentEdgeNormal = glm::cross(axisFace, currentEdge.second - currentEdge.first);
                // flip normal if pointing in the wrong direction
                if (glm::dot(currentEdge.first - triangle->center, currentEdgeNormal) < 0.f)
                {
                    currentEdgeNormal = -currentEdgeNormal;
                }
                float d =   -currentEdgeNormal.x * currentEdge.first.x - \
                            -currentEdgeNormal.y * currentEdge.first.y - \
                            -currentEdgeNormal.z * currentEdge.first.z;
                std::vector<glm::vec3> temp = this->Clip(edgesB, std::make_pair(currentEdgeNormal, d));
                result.insert(result.end(), temp.begin(), temp.end());
            }
            // clip against triangle face.
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

bool CollisionDetector::IsSeparatingAxis(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB, glm::vec3 direction, float& tempPenDepth)
{
    float minA = glm::dot(direction, this->GetSupportPoint(pointsA, -direction));
    float maxA = glm::dot(direction, this->GetSupportPoint(pointsA, direction));
    float minB = glm::dot(direction, this->GetSupportPoint(pointsB, -direction));
    float maxB = glm::dot(direction, this->GetSupportPoint(pointsB, direction));
    if (minA >= maxB || maxA <= minB)
    {
        return true;
    }
    float first = maxB - minA;
    float second = maxA - minB;
    if (second < first)
        tempPenDepth = second;
    else
        tempPenDepth = first;
    return false;
}

float CollisionDetector::GetShortestDistanceBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, const std::pair<glm::vec3, glm::vec3>& edgeB)
{
    glm::vec3 d1 = edgeA.second - edgeA.first;
    glm::vec3 d2 = edgeB.second - edgeB.first;
    glm::vec3 r  = edgeA.first - edgeB.first;
    float a = glm::dot(d1, d1);
    float b = glm::dot(d1, d2);
    float c = glm::dot(d1, r);
    float e = glm::dot(d2, d2);
    float f = glm::dot(d2, r);
    float d = a * e - b * b;
    assert(d != 0.f);

    float s = (b * f - c * e) / d;
    float t = (a * f - b * c) / d;

    glm::vec3 l1 = edgeA.first + s * d1;
    glm::vec3 l2 = edgeB.first + t * d2;
    return glm::length(l2 - l1);
}

std::vector<glm::vec3> CollisionDetector::Clip(const std::vector<std::pair<glm::vec3, glm::vec3>>& edges,  std::pair<glm::vec3, float> plane)
{
    // Used Sutherland-Hodgman for the clipping
    // https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

    // NOTE : output may contain multiples of a single point.
    std::vector<glm::vec3> output;
    for (int i = 0; i < edges.size(); i++)
    {
        // compute intersection points
        glm::vec3 v1 = edges[i].first;
        glm::vec3 v2 = edges[i].second;
        glm::vec3 intersectionPoint;
        if (this->IntersectLinePlane(v1, v2, plane, intersectionPoint))
        {
            if (glm::dot(plane.first, v2) < 0)
            {
                if (glm::dot(plane.first, v1) > 0)
                {
                    output.push_back(intersectionPoint);
                }
                output.push_back(v2);
            }
            else if (glm::dot(plane.first, v1) < 0)
            {
                output.push_back(intersectionPoint);
            }
        }
        // check positions of points
    }
    return output;
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

bool CollisionDetector::IntersectLinePlane(glm::vec3 a, glm::vec3 b, std::pair<glm::vec3, float> plane, glm::vec3& result)
{
    // line 175 of real-time collision detection.
    glm::vec3 ab = b - a;
    float t = (plane.second - glm::dot(plane.first, a)) / (glm::dot(plane.first, ab));
    if (t >= 0.f && t <= 1.0f)
    {
        result = a + t * ab;
        return true;
    }
    return false;
}