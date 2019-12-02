#include <math.h>
#include <memory>
#include <iostream>
#include <unordered_set>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "CollisionDetector.hpp"
#include "../../util.hpp"


CollisionDetector::CollisionDetector()
{

}

std::shared_ptr<Collision> CollisionDetector::Collide(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
    std::vector<glm::vec3> collisionPoints;

    // get edges and faces
    const std::vector<glm::vec3>& pointsA                       = first->GetPoints();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& facesA  = first->GetFaces();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edgesA  = first->GetEdges();

    const std::vector<glm::vec3>& pointsB                       = second->GetPoints();
    const std::vector<std::pair<glm::vec3, glm::vec3>>& facesB  = second->GetFaces();
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

    if (this->CheckFaces(data, pointsA, pointsB, facesA, first, second, true))
        return nullptr;

    if (this->CheckFaces(data, pointsA, pointsB, facesB, first, second, false))
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
            collisionPoints = this->GetCollisionPoints(data, edgesB, facesA);
            data.collisionAxis = -data.collisionAxis;
        }
        else
        {
            collisionPoints = this->GetCollisionPoints(data, edgesA, facesB);
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
    glm::vec3 e1 = edgeA.second - edgeA.first;
    glm::vec3 e2 = edgeB.second - edgeB.first;
    glm::vec3 r = edgeB.first - edgeA.first;
    glm::vec3 normal = glm::cross(e1, e2);
    glm::vec3 normal1 = glm::cross(e1, normal);
    glm::vec3 normal2 = glm::cross(e2, normal);
    glm::vec3 p1 = edgeA.first + (glm::dot(r, normal2)/glm::dot(e1, normal2)) * e1;
    glm::vec3 p2 = edgeB.first + (glm::dot(-r, normal1)/glm::dot(e2, normal1)) * e2;
    return p1 + 0.5f * (p2 - p1);
}

float CollisionDetector::GetMinDistanceBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, const std::pair<glm::vec3, glm::vec3>& edgeB)
{
    glm::vec3 e1 = edgeA.second - edgeA.first;
    glm::vec3 e2 = edgeB.second - edgeB.first;
    glm::vec3 unitE1 = glm::normalize(e1);
    glm::vec3 unitE2 = glm::normalize(e2);
    glm::vec3 w = edgeA.first - edgeB.first;
    float a = glm::dot(e1, e1);
    float b = glm::dot(e1, e2);
    float c = glm::dot(e2, e2);
    float d = glm::dot(e1, w);
    float e = glm::dot(e2, w);
    float D = a*c - b*b;
    float sc, sNominator, sDenominator = D;
    float tc, tNominator, tDenominator = D;
    
    if (D < 0.0005f)
    {
        sNominator      = 0.f;
        sDenominator    = 1.f;
        tNominator      = e;
        tDenominator    = c;
    }
    else
    {
        sNominator = b*e - c*d;
        tNominator = a*e - b*d;
        if (sNominator < 0.f)   // closest point is closer to s=0
        {
            sNominator = 0.f;
            tNominator = e;
            tDenominator = c;
        }
        else if (sNominator > sDenominator)   // closest point is closer to s=1
        {
            sNominator = sDenominator;
            tNominator = e + b;
            tDenominator = c;
        }
    }

    if (tNominator  < 0.f)
    {
        tNominator = 0.f;
        if (-d < 0.f)
        {
            sNominator = 0.f;
        }
        else if (-d > a)
        {
            sNominator = sDenominator;
        }
        else
        {
            sNominator = -d;
            sDenominator = a;
        }
    }
    else if (tNominator > tDenominator)
    {
        tNominator = tDenominator;
        if ((-d + b) < 0.f)
        {
            sNominator = 0.f;
        }
        else if ((-d + b) > a)
        {
            sNominator = sDenominator;
        }
        else
        {
            sNominator = -d + b;
            sDenominator = a;
        }
    }
    sc = (abs(sNominator) < 0.0005f ? 0.0 : sNominator / sDenominator);
    tc = (abs(tNominator) < 0.0005f ? 0.0 : tNominator / tDenominator);
    glm::vec3 result = w + sc * e1 - tc * e2;
    // dont need actual distance
    return glm::length2(result);
}

std::vector<glm::vec3> CollisionDetector::Clip(const std::vector<std::pair<glm::vec3, glm::vec3>>& edges, const std::vector<std::pair<glm::vec3, glm::vec3>>& planes)
{
    std::vector<glm::vec3> output;
    for (int k = 0; k < edges.size(); k++)
    {
        glm::vec3 v1 = edges[k].first;
        glm::vec3 v2 = edges[k].second;

        bool shouldAddFirst = !this->ContainsPoint(v1, output);
        bool shouldAddSecond = !this->ContainsPoint(v2, output);

        if (!shouldAddFirst && !shouldAddSecond)
        {
            continue;
        }

        glm::vec3 intersectionPoint = glm::vec3(0.f, 0.f, 0.f);
        for (int j = 0; j < planes.size(); j++)
        {
            std::pair<glm::vec3, glm::vec3> plane = planes[j];
            float dotFirst = glm::dot(plane.first, v1);
            float dotSecond = glm::dot(plane.first, v2);
            float planeOffset = glm::dot(plane.first, plane.second);
            if (dotFirst > planeOffset)
            {
                shouldAddFirst = false;
            }
            if (dotSecond > planeOffset)
            {
                shouldAddSecond = false;
            }
            if (dotFirst < planeOffset && dotSecond > planeOffset || dotFirst > planeOffset && dotSecond < planeOffset)
            {
                glm::vec3 intersectionPoint = this->IntersectLinePlane(v1, v2, plane);
            }
        }

        if (shouldAddFirst)
        {
            output.push_back(v1);
        }
        if (shouldAddSecond)
            output.push_back(v2);
        float magnitudeSq = glm::length2(intersectionPoint);
        if ((shouldAddFirst || shouldAddSecond) &&  !(shouldAddFirst && shouldAddSecond) && magnitudeSq > 0.f)
        {
            output.push_back(intersectionPoint);
        }
    }
    return output;
}

bool CollisionDetector::ContainsPoint(glm::vec3& point, std::vector<glm::vec3>& points)
{
    for (int i = 0; i < points.size(); i++)
    {
        if (glm::all(glm::equal(point, points[i])))
            return true;
    }
    return false;
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

glm::vec3 CollisionDetector::IntersectLinePlane(glm::vec3 a, glm::vec3 b, std::pair<glm::vec3, glm::vec3> plane)
{
    // line 175 of real-time collision detection.
    glm::vec3 ab = b - a;
    glm::vec3 result = glm::vec3(0.f,0.f,0.f);
    float planeOffset = glm::dot(plane.first, plane.second);
    float t = (planeOffset - glm::dot(plane.first, a)) / (glm::dot(plane.first, ab));
    if (t >= 0.f && t <= 1.0f)
    {
        result = a + t * ab;
    }
    return result;
}

bool CollisionDetector::CheckFaces( SATData& data, 
                                    const std::vector<glm::vec3>& pointsA, 
                                    const std::vector<glm::vec3>& pointsB,
                                    const std::vector<std::pair<glm::vec3, glm::vec3>>& faces,
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
            // if (currPenDepth <= data.minPenDepth)
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
                                                             const std::vector<std::pair<glm::vec3, glm::vec3>>& edges,
                                                             const std::vector<std::pair<glm::vec3, glm::vec3>>& faces)
{
    std::vector<glm::vec3> collisionPoints;
    // get all 4 planes to clip against
    // clip the remaining points against the plane of the AABB with the same normal as the axisFase
    std::vector<std::pair<glm::vec3, glm::vec3>> planes;
    for (int i = 0; i < faces.size(); i++)
    {
        glm::vec3 normal = faces[i].first;
        if (glm::dot(normal, data.collisionAxis) == 0.0f)
        {
            planes.push_back(faces[i]);
        }
    }
    std::vector<glm::vec3> clippedPoints = this->Clip(edges, planes);
    for (int i = 0; i < clippedPoints.size(); i++)
    {
        float dotFaceOne = glm::dot(data.collisionAxis, clippedPoints[i] - faces[data.indexFace].second);
        if ( dotFaceOne <= 0.f)
        {
            collisionPoints.push_back(clippedPoints[i]);
        }
    }
    return collisionPoints;
}