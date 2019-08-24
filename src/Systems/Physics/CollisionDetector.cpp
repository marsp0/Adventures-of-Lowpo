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
    if (first->colliderType == ColliderType::BOX && second->colliderType == ColliderType::BOX)
    {
        std::shared_ptr<AABB> firstBox = std::static_pointer_cast<AABB>(first);
        std::shared_ptr<AABB> secondBox = std::static_pointer_cast<AABB>(second);
        return this->AABBToAABB(firstBox, secondBox);
    }
    return this->Collide(first, second);
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

std::shared_ptr<Collision> CollisionDetector::Collide(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
    int indexFace;
    int indexEdgeA;
    int indexEdgeB;

    bool indexFaceA = false;
    bool isFaceCollision = false;

    // collision axis on which the penetration is minimal
    glm::vec3 collisionAxis;
    float currPenDepth = 0.f;
    float minPenDepth = 10000.f;
    float minEdgeDistance = 10000.f;

    std::vector<glm::vec3> collisionPoints;

    // get edges and faces
    const std::vector<glm::vec3>& pointsA                       = first->GetPoints();
    const std::vector<glm::vec3>& pointsOnFacesA                = first->GetPointsOnFaces();
    const std::vector<std::pair<glm::vec3, float>>&     facesA  = first->GetFaces();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesA  = first->GetEdges();

    const std::vector<glm::vec3>& pointsB                       = second->GetPoints();
    const std::vector<glm::vec3>& pointsOnFacesB                = second->GetPointsOnFaces();
    const std::vector<std::pair<glm::vec3, float>>&     facesB  = second->GetFaces();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesB  = second->GetEdges();

    for (int i = 0; i < facesA.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesA[i].first, currPenDepth))
        {
            return nullptr;
        }
        bool sameDirection = glm::dot(first->center - second->center, facesA[i].first) > 0.f;
        if (currPenDepth < minPenDepth || (currPenDepth == minPenDepth && sameDirection))
        {   
            indexFace = i;
            indexFaceA = true;
            isFaceCollision = true;
            collisionAxis = facesA[i].first;
            minPenDepth = currPenDepth;
        }
    }

    for (int i = 0; i < facesB.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, facesB[i].first, currPenDepth))
        {
            return nullptr;
        }
        bool sameDirection = glm::dot(first->center - second->center, facesA[i].first) > 0.f;
        if (currPenDepth < minPenDepth || (currPenDepth == minPenDepth && sameDirection))
        {
            indexFace = i;
            indexFaceA = false;
            isFaceCollision = true;
            collisionAxis = facesB[i].first;
            minPenDepth = currPenDepth; 
        }
    }

    for (int i = 0; i < edgesA.size(); i++)
    {
        for (int j = 0; j < edgesB.size(); j++)
        {
            // take the cross
            glm::vec3 edgeA = edgesA[i].second - edgesA[i].first;
            glm::vec3 edgeB = edgesB[j].second - edgesB[j].first;
            glm::vec3 possibleCollisionAxis = glm::cross(edgeA, edgeB);

            // maintain normal orientation
            if (glm::dot(edgesA[i].first - first->center, possibleCollisionAxis) < 0)
            {
                possibleCollisionAxis = -possibleCollisionAxis;
            }
            // SAT check
            if (this->IsSeparatingAxis(pointsA, pointsB, possibleCollisionAxis, currPenDepth) && glm::length(possibleCollisionAxis) > 0.f)
            {
                return nullptr;
            }
            // we need this as we can have multiple edges with the same penetration depth,
            // but where the actual distance between the edges is different. AABB one such collider
            float currMinDistance = this->GetMinDistanceBetweenEdges(edgesA[i], edgesB[j]);
            if (currPenDepth <= minPenDepth && currMinDistance < minEdgeDistance)
            {
                // Add this check to the if above to make sure that we get the correct edge
                indexEdgeA = i;
                indexEdgeB = j;

                isFaceCollision = false;
                collisionAxis   = possibleCollisionAxis;

                minPenDepth     = currPenDepth;
                minEdgeDistance = currMinDistance;
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
                if (glm::dot(normal, collisionAxis) == 0.0f)
                {
                    planes.push_back(facesA[i]);
                }
            }
            // find the 2 faces that are on the same axis
            int secondFaceIndex;
            for (int i = 0; i < facesA.size(); i++)
            {
                float dotProduct = glm::dot(facesA[i].first, collisionAxis);
                if (dotProduct == -1.0f)
                {
                    secondFaceIndex = i;
                }
            }
            std::vector<glm::vec3> clippedPoints = this->Clip(pointsB, planes);
            for (int i = 0; i < clippedPoints.size(); i++)
            {
                float dotFaceOne = glm::dot(collisionAxis, clippedPoints[i] - pointsOnFacesA[indexFace]);
                float dotFaceTwo = glm::dot(facesA[secondFaceIndex].first, clippedPoints[i] - pointsOnFacesA[secondFaceIndex]);
                if ( dotFaceOne <= 0.f && dotFaceTwo <= 0.f)
                {
                    collisionPoints.push_back(clippedPoints[i]);
                }
            }
        }
        else
        {
            // get all 4 planes to clip against
            // clip the remaining points against the plane of the AABB with the same normal as the axisFase
            std::vector<std::pair<glm::vec3, float>> planes;
            for (int i = 0; i < facesB.size(); i++)
            {
                glm::vec3 normal = facesB[i].first;
                if (glm::dot(normal, collisionAxis) == 0.0f)
                {
                    planes.push_back(facesB[i]);
                }
            }
            // find the 2 faces that are on the same axis
            int secondFaceIndex;
            for (int i = 0; i < facesB.size(); i++)
            {
                float dotProduct = glm::dot(facesB[i].first, collisionAxis);
                if (dotProduct == -1.0f)
                {
                    secondFaceIndex = i;
                }
            }
            std::vector<glm::vec3> clippedPoints = this->Clip(pointsA, planes);
            for (int i = 0; i < clippedPoints.size(); i++)
            {
                float dotFaceOne = glm::dot(collisionAxis, clippedPoints[i] - pointsOnFacesB[indexFace]);
                float dotFaceTwo = glm::dot(facesA[secondFaceIndex].first, clippedPoints[i] - pointsOnFacesB[secondFaceIndex]);
                if ( dotFaceOne <= 0.f && dotFaceTwo <= 0.f)
                {
                    collisionPoints.push_back(clippedPoints[i]);
                }
            }
        }
    }
    std::vector<Contact> contacts;
    std::shared_ptr<Collision> collision = std::make_shared<Collision>(first->entityID, first, second->entityID, second, contacts);
    for (int i = 0; i < collisionPoints.size(); i++)
    {
        glm::vec3 point = collisionPoints[i];
        Contact contact = Contact(point, collisionAxis, minPenDepth);
        collision->contacts.push_back(contact);
    }
    return collision;
}

bool CollisionDetector::IsSeparatingAxis(const std::vector<glm::vec3>& pointsA, const std::vector<glm::vec3>& pointsB, glm::vec3 direction, float& tempPenDepth)
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
    glm::vec3 d1 = edgeA.second - edgeA.first;
    glm::vec3 d2 = edgeB.second - edgeB.first;
    glm::vec3 r = edgeB.first - edgeA.first;
    glm::vec3 normal = glm::cross(d1, d2);
    glm::vec3 normal1 = glm::cross(d1, normal);
    glm::vec3 normal2 = glm::cross(d2, normal);
    glm::vec3 p1 = edgeA.first + (glm::dot(r, normal2)/glm::dot(d1, normal2)) * d1;
    glm::vec3 p2 = edgeB.first + (glm::dot(-r, normal1)/glm::dot(d2, normal1)) * d2;
    return p1 + 0.5f * (p2 - p1);
}

float CollisionDetector::GetMinDistanceBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, const std::pair<glm::vec3, glm::vec3>& edgeB)
{
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
        return shortestDistance;
    }
    // do skew lines here
    // https://www.quora.com/How-do-I-find-the-shortest-distance-between-two-skew-lines
    glm::vec3 crossProduct = glm::cross(d1, d2);
    float shortestDistance = glm::dot(r, crossProduct);
    return shortestDistance;
}

std::vector<glm::vec3> CollisionDetector::Clip(const std::vector<glm::vec3> points, std::vector<std::pair<glm::vec3, float>> planes)
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

glm::vec3 CollisionDetector::GetSupportPoint(const std::vector<glm::vec3>& points, glm::vec3 direction)
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