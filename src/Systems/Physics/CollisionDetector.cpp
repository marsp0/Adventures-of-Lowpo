#include <math.h>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "CollisionDetector.hpp"


CollisionDetector::CollisionDetector()
{

}

std::shared_ptr<Collision> CollisionDetector::CheckCollision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
    if (first->colliderType == ColliderType::BOX && second->colliderType == ColliderType::TRIANGLE || \
        first->colliderType == ColliderType::TRIANGLE && second->colliderType == ColliderType::BOX)
    {
        if (first->colliderType == ColliderType::BOX)
        {
            std::shared_ptr<AABB> box = std::static_pointer_cast<AABB>(first);
            std::shared_ptr<Triangle> triangle = std::static_pointer_cast<Triangle>(second);
            return this->AABBToTriangle(box,triangle);
        }
        std::shared_ptr<AABB> box = std::static_pointer_cast<AABB>(second);
        std::shared_ptr<Triangle> triangle = std::static_pointer_cast<Triangle>(first);
        return this->AABBToTriangle(box,triangle);
    }
    else if (first->colliderType == ColliderType::BOX && second->colliderType == ColliderType::BOX)
    {
        std::shared_ptr<AABB> firstBox = std::static_pointer_cast<AABB>(first);
        std::shared_ptr<AABB> secondBox = std::static_pointer_cast<AABB>(second);
        return this->AABBToAABB(firstBox, secondBox);
    }
}

std::shared_ptr<Collision> CollisionDetector::AABBToAABB(std::shared_ptr<AABB> first, std::shared_ptr<AABB> second)
{
    std::vector<glm::vec3> pointsA = first->GetPoints();
    std::vector<glm::vec3> pointsB = second->GetPoints();
    const std::vector<glm::vec3>   pointsOnFacesA = first->GetPointsOnFaces();
    const std::vector<std::pair<glm::vec3, float>> facesA = first->GetFaces();

    int faceIndex;
    glm::vec3 separatingAxis;
    float tempMinPenetration = 0.f;
    float minPenetration = 100000.0f;

    std::vector<glm::vec3> collisionPoints;

    for (int i = 0; i < facesA.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesA[i].first, tempMinPenetration))
        {
            return nullptr;
        }
        if (tempMinPenetration < minPenetration)
        {
            minPenetration = tempMinPenetration;
            faceIndex = i;
            separatingAxis = facesA[i].first;
        }
    }
    for (int i = 0; i < pointsB.size(); i++)
    {
        float dotProduct = glm::dot(separatingAxis, pointsB[i] - pointsOnFacesA[faceIndex]);
        if (dotProduct <= 0.f)
        {
            collisionPoints.push_back(pointsB[i]);
        }
    }
    std::vector<Contact> contacts;
    std::shared_ptr<Collision> collision = std::make_shared<Collision>(first->entityID, first, second->entityID, second, contacts);
    for (int i = 0; i < collisionPoints.size() ; i++)
    {
        Contact contact = Contact(collisionPoints[i], separatingAxis, minPenetration);
        collision->contacts.push_back(contact);
    }
    return collision;
}

std::shared_ptr<Collision> CollisionDetector::AABBToTriangle(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle)
{
    int indexFace;
    int indexEdgeA;
    int indexEdgeB;

    bool indexFaceA = false;
    bool updateAxis = false;
    bool isFaceCollision = false;

    glm::vec3 separatingAxis;
    
    float tempPenetrationDepth = 0.f;
    float minPenetrationDepth = 10000.f;
    float minDistanceBetweenEdges = 10000.f;

    std::vector<glm::vec3> collisionPoints;

    // NEW IMPLEMENTATION
    // get edges and faces
    std::vector<glm::vec3> pointsA = box->GetPoints();
    const std::vector<glm::vec3> pointsOnFacesA = box->GetPointsOnFaces();
    const std::vector<std::pair<glm::vec3, float>>&     facesA = box->GetFaces();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesA = box->GetEdges();

    std::vector<glm::vec3> pointsB = triangle->GetPoints();
    const std::vector<glm::vec3> pointsOnFacesB = triangle->GetPointsOnFaces();
    const std::vector<std::pair<glm::vec3, float>>&     facesB = triangle->GetFaces();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesB = triangle->GetEdges();

    // TODO : refactor this into a function
    for (int i = 0; i < facesA.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesA[i].first, tempPenetrationDepth))
        {
            return nullptr;
        }
        bool smaller = tempPenetrationDepth < minPenetrationDepth;
        bool equal = tempPenetrationDepth == minPenetrationDepth;
        bool sameDir = glm::dot(triangle->center - box->center, facesA[i].first) > 0.f;
        if (smaller || equal && sameDir)
        {   
            indexFace = i;
            indexFaceA = true;
            isFaceCollision = true;
            separatingAxis = facesA[i].first;
            minPenetrationDepth = tempPenetrationDepth;
        }
    }

    // TODO : refactor this into a function
    for (int i = 0; i < facesB.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesB[i].first, tempPenetrationDepth))
        {
            return nullptr;
        }
        if (tempPenetrationDepth < minPenetrationDepth)
        {
            indexFace = i;
            indexFaceA = false;
            isFaceCollision = true;
            separatingAxis = facesB[i].first;
            minPenetrationDepth = tempPenetrationDepth; 
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
            glm::vec3 possibleSeparatingAxis = glm::cross(edgeA, edgeB);

            // maintain normal orientation
            if (glm::dot(edgesA[i].first - box->center, possibleSeparatingAxis) < 0)
            {
                possibleSeparatingAxis = -possibleSeparatingAxis;
            }
            // SAT check
            if (this->IsSeparatingAxis(pointsA, pointsB, possibleSeparatingAxis, tempPenetrationDepth) && glm::length(possibleSeparatingAxis) > 0.f)
            {
                return nullptr;
            }
            // we need this as we can have multiple edges with the same penetration depth,
            // but where the actual distance between the edges is different. AABB one such collider
            glm::vec3 shortestVec = this->ShortestVectorBetweenEdges(edgesA[i], edgesB[j]);
            float edgeDistance = glm::length(shortestVec);
            if (tempPenetrationDepth <= minPenetrationDepth && edgeDistance < minDistanceBetweenEdges)
            {
                // Add this check to the if above to make sure that we get the correct edge
                indexEdgeA = i;
                indexEdgeB = j;

                isFaceCollision = false;
                separatingAxis  = possibleSeparatingAxis;

                minPenetrationDepth     = tempPenetrationDepth;
                minDistanceBetweenEdges = edgeDistance;
            }
        }
    } 
    // check if contact is edge edge or face whatever
    if (!isFaceCollision)
    {
        // TODO : Refactor this part as the same chunk is present in ShortestDistanceBetweenEdges
        std::pair<glm::vec3, glm::vec3> edgeA = edgesA[indexEdgeA];
        std::pair<glm::vec3, glm::vec3> edgeB = edgesB[indexEdgeB];
        collisionPoints.push_back(this->GetContactBetweenEdges(edgeA, edgeB));
    }
    else
    {
        // face face contact logic here
        // normal is the axis
        if (indexFaceA)
        {
            // get all 4 planes to clip against
            // clip the remaining points against the plane of the AABB with the same normal as the axisFase
            std::vector<std::pair<glm::vec3, float>> planes;
            for (int i = 0; i < facesA.size(); i++)
            {
                glm::vec3 normal = facesA[i].first;
                if (glm::dot(normal, separatingAxis) == 0.0f)
                {
                    planes.push_back(facesA[i]);
                }
            }
            // find the 2 faces that are on the same axis
            int secondFaceIndex;
            for (int i = 0; i < facesA.size(); i++)
            {
                float dotProduct = glm::dot(facesA[i].first, separatingAxis);
                if (dotProduct == -1.0f)
                {
                    secondFaceIndex = i;
                }
            }
            std::vector<glm::vec3> clippedPoints = this->Clip(pointsB, planes);
            for (int i = 0; i < clippedPoints.size(); i++)
            {
                float dotFaceOne = glm::dot(separatingAxis, clippedPoints[i] - pointsOnFacesA[indexFace]);
                float dotFaceTwo = glm::dot(facesA[secondFaceIndex].first, clippedPoints[i] - pointsOnFacesA[secondFaceIndex]);
                if ( dotFaceOne <= 0.f && dotFaceTwo <= 0.f)
                {
                    collisionPoints.push_back(clippedPoints[i]);
                }
                
            }
        }
        else
        {
            for (int i = 0; i < pointsA.size(); i++)
            {
                float dotProduct = glm::dot(separatingAxis, pointsA[i] - pointsB[0]);
                if (dotProduct <= 0.f)
                {
                    collisionPoints.push_back(pointsA[i]);
                }
            }
        }
    }
    std::vector<Contact> contacts;
    std::shared_ptr<Collision> collision = std::make_shared<Collision>(box->entityID, box, triangle->entityID, triangle, contacts);
    for (int i = 0; i < collisionPoints.size(); i++)
    {
        glm::vec3 point = collisionPoints[i];
        Contact contact = Contact(point, separatingAxis, minPenetrationDepth);
        collision->contacts.push_back(contact);
    }
    return collision;
}


std::shared_ptr<Collision> CollisionDetector::TriangleToAABB(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box)
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

glm::vec3 CollisionDetector::GetContactBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, const std::pair<glm::vec3, glm::vec3>& edgeB)
{
    // TODO (Martin) : refactor to match the method below
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
    return l1 + 0.5f * (l2 - l1);
}

glm::vec3 CollisionDetector::ShortestVectorBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, const std::pair<glm::vec3, glm::vec3>& edgeB)
{
    // experimental
    float epsilon = 0.0005f;
    glm::vec3 d1 = glm::normalize(edgeA.second - edgeA.first);
    glm::vec3 d2 = glm::normalize(edgeB.second - edgeB.first);
    glm::vec3 r  = edgeA.first - edgeB.first;
    glm::vec3 result = glm::vec3(0.f, 0.f, 0.f);
    float diff = std::abs(glm::dot(d1, d2) - 1.f);
    if (diff < epsilon)
    {
        // do parallel lines here
        float rMagnitude = glm::length(r);
        float rProjectionMagnitude = glm::dot(r, d2);
        glm::vec3 rProjection = rProjectionMagnitude * d2;
        float shortestDistance = std::sqrt(std::pow(rMagnitude, 2.f) - std::pow(rProjectionMagnitude, 2.f));
        result = glm::normalize(r - rProjection) * shortestDistance;
        return result;
    }
    // do skew lines here
    // https://www.quora.com/How-do-I-find-the-shortest-distance-between-two-skew-lines
    glm::vec3 crossProduct = glm::cross(d1, d2);
    float shortestDistance = glm::dot(r, crossProduct);
    result = crossProduct * shortestDistance;
    return result;
}

std::vector<glm::vec3> CollisionDetector::Clip(std::vector<glm::vec3> points, std::vector<std::pair<glm::vec3, float>> planes)
{
    // Used Sutherland-Hodgman for the clipping
    // https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

    // NOTE : output may contain multiples of a single point.
    std::vector<glm::vec3> output = points;
    for (int j = 0; j < planes.size(); j++)
    {
        std::vector<glm::vec3> input = output;
        output.clear();
        std::pair<glm::vec3, float> plane = planes[j];
        for (int i = 0; i < input.size(); i++)
        {
            // compute intersection points
            glm::vec3 v1 = input[i];
            glm::vec3 v2 = input[(i+1) % input.size()];
            glm::vec3 intersectionPoint = this->IntersectLinePlane(v1, v2, plane);
            if (glm::dot(plane.first, v2) < plane.second)
            {
                if (glm::dot(plane.first, v1) > plane.second)
                {
                    output.push_back(intersectionPoint);
                }
                output.push_back(v2);
            }
            else if (glm::dot(plane.first, v1) < plane.second)
            {
                output.push_back(intersectionPoint);
            }
            // check positions of points
        }
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

glm::vec3 CollisionDetector::IntersectLinePlane(glm::vec3 a, glm::vec3 b, std::pair<glm::vec3, float> plane)
{
    // line 175 of real-time collision detection.
    glm::vec3 ab = b - a;
    glm::vec3 result = glm::vec3(0.f,0.f,0.f);
    float t = (plane.second - glm::dot(plane.first, a)) / (glm::dot(plane.first, ab));
    if (t >= 0.f && t <= 1.0f)
    {
        result = a + t * ab;
    }
    return result;
}

glm::vec3 CollisionDetector::ProjectPointOntoPlane(glm::vec3 point, std::pair<glm::vec3, float> plane)
{
    glm::vec3 normal = plane.first;
    glm::vec3 pointOnPlane = normal * plane.second;
    glm::vec3 r = point - (glm::dot(normal, point - pointOnPlane)) * plane.first;
    return r;
}