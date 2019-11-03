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
    return this->Collide(first, second);
}

std::shared_ptr<Collision> CollisionDetector::Collide(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
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

    SATData data;
    data.indexFaceA = false;
    data.isFaceCollision = false;
    data.minPenDepth = 10000.f;
    data.minEdgeDistance = 10000.f;

    // checking edges first in case we end up with cross product that represents the normal
    // of a face. This causes us to have a face collision represented as an edge collision.
    // Need to fix that, but atm i have no idea how.
    // For concrete example check GridTest.cpp - collision between plane and box (entityID 1 and 3)
    if (this->CheckEdges(data, pointsA, pointsB, edgesA, edgesB))
        return nullptr;

    if (this->CheckFaces(data, pointsA, pointsB, facesA, pointsOnFacesA, first, second, true))
        return nullptr;

    if (this->CheckFaces(data, pointsA, pointsB, facesB, pointsOnFacesB, first, second, false))
        return nullptr;

    if (!data.isFaceCollision)
    {
        // TODO : Refactor this part as the same chunk is present in ShortestDistanceBetweenEdges
        std::pair<glm::vec3, glm::vec3> edgeA = edgesA[data.indexEdgeA];
        std::pair<glm::vec3, glm::vec3> edgeB = edgesB[data.indexEdgeB];
        collisionPoints.push_back(this->GetContactBetweenEdges(edgeA, edgeB));

        // adjust the direction of the normal.
        float direction = glm::dot(second->center - first->center, data.collisionAxis);
        if (direction > 0.f)
            data.collisionAxis = -data.collisionAxis;
    }
    else
    {
        // face face contact logic here
        // normal is the axis
        if (data.indexFaceA)
        {
            collisionPoints = this->GetCollisionPoints(data, pointsB, facesA, pointsOnFacesA);
            data.collisionAxis = -data.collisionAxis;
        }
        else
        {
            collisionPoints = this->GetCollisionPoints(data, pointsA, facesB, pointsOnFacesB);
        }
    }
    std::vector<Contact> contacts;
    std::shared_ptr<Collision> collision = std::make_shared<Collision>(first->entityID, first, second->entityID, second, contacts);
    for (int i = 0; i < collisionPoints.size(); i++)
    {
        glm::vec3 point = collisionPoints[i];
        Contact contact = Contact(point, data.collisionAxis, data.minPenDepth);
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

bool CollisionDetector::CheckFaces( SATData& data, 
                                    const std::vector<glm::vec3>& pointsA, 
                                    const std::vector<glm::vec3>& pointsB,
                                    const std::vector<std::pair<glm::vec3, float>>& faces,
                                    const std::vector<glm::vec3>& pointsOnFaces,
                                    std::shared_ptr<Collider> first,
                                    std::shared_ptr<Collider> second,
                                    bool isFaceA)
{
    // used to make sure we get the correct face
    // AABBs have faces with same axis as normal but different directions.
    glm::vec3 centerDir;
    if (isFaceA)
        centerDir = second->center - first->center;
    else
        centerDir = first->center - second->center;

    float currPenDepth = 0.f;
    for (int i = 0; i < faces.size(); i++)
    {
        if (this->IsSeparatingAxis(pointsA, pointsB, faces[i].first, currPenDepth))
        {
            return true;
        }
        float sameDirAsCenters = glm::dot(centerDir, faces[i].first);
        if (currPenDepth <= data.minPenDepth && sameDirAsCenters >= 0.f)
        {
            data.indexFace = i;
            data.indexFaceA = isFaceA;
            data.isFaceCollision = true;
            data.collisionAxis = faces[i].first;
            data.minPenDepth = currPenDepth;
        }
    }
    return false;
}

bool CollisionDetector::CheckEdges( SATData& data,
                                    const std::vector<glm::vec3>& pointsA, 
                                    const std::vector<glm::vec3>& pointsB,
                                    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesA,
                                    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesB)
{
    float currPenDepth = 0.f;
    for (int i = 0; i < edgesA.size(); i++)
    {
        for (int j = 0; j < edgesB.size(); j++)
        {
            // take the cross
            glm::vec3 edgeA = edgesA[i].second - edgesA[i].first;
            glm::vec3 edgeB = edgesB[j].second - edgesB[j].first;
            glm::vec3 possibleCollisionAxis = glm::normalize(glm::cross(edgeA, edgeB));

            if (glm::length(possibleCollisionAxis) < 0.005f)
                continue;

            // SAT check
            if (this->IsSeparatingAxis(pointsA, pointsB, possibleCollisionAxis, currPenDepth))
            {
                return true;
            }
            // we need this as we can have multiple edges with the same penetration depth,
            // but where the actual distance between the edges is different. AABB one such collider
            float currMinDistance = this->GetMinDistanceBetweenEdges(edgesA[i], edgesB[j]);
            if (currPenDepth <= data.minPenDepth && currMinDistance < data.minEdgeDistance)
            {
                // Add this check to the if above to make sure that we get the correct edge
                data.indexEdgeA = i;
                data.indexEdgeB = j;

                data.isFaceCollision = false;
                data.collisionAxis   = possibleCollisionAxis;

                data.minPenDepth     = currPenDepth;
                data.minEdgeDistance = currMinDistance;
            }
        }
    }
    return false;
}

std::vector<glm::vec3> CollisionDetector::GetCollisionPoints(SATData& data, 
                                                             const std::vector<glm::vec3>& points,
                                                             const std::vector<std::pair<glm::vec3, float>>& faces,
                                                             const std::vector<glm::vec3>& pointsOnFaces)
{
    std::vector<glm::vec3> collisionPoints;
    // get all 4 planes to clip against
    // clip the remaining points against the plane of the AABB with the same normal as the axisFase
    std::vector<std::pair<glm::vec3, float>> planes;
    for (int i = 0; i < faces.size(); i++)
    {
        glm::vec3 normal = faces[i].first;
        if (glm::dot(normal, data.collisionAxis) == 0.0f)
        {
            planes.push_back(faces[i]);
        }
    }
    std::vector<glm::vec3> clippedPoints = this->Clip(points, planes);
    for (int i = 0; i < clippedPoints.size(); i++)
    {
        float dotFaceOne = glm::dot(data.collisionAxis, clippedPoints[i] - pointsOnFaces[data.indexFace]);
        if ( dotFaceOne <= 0.f)
        {
            collisionPoints.push_back(clippedPoints[i]);
        }
    }
    return collisionPoints;
}