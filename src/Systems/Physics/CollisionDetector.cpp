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
    const std::vector<glm::vec3>&           pointsA = first->GetPoints();
    const std::vector<std::pair<int, int>>& edgesA  = first->GetEdges();
    const std::vector<ColliderFace>&        facesA = first->GetFaces();

    const std::vector<glm::vec3>&           pointsB = second->GetPoints();
    const std::vector<std::pair<int, int>>& edgesB  = second->GetEdges();
    const std::vector<ColliderFace>&        facesB = second->GetFaces();

    SATData data;
    data.isFaceACollision = false;
    data.isFaceCollision = false;
    data.minPenDepth = 10000.f;
    data.minEdgeDistance = 10000.f;

    // checking edges first in case we end up with cross product that represents the normal
    // of a face. This causes us to have a face collision represented as an edge collision.
    // Need to fix that, but atm i have no idea how.
    // For concrete example check GridTest.cpp - collision between plane and box (entityID 1 and 3)
    if (this->CheckEdges(data, first, second))
        return nullptr;

    if (this->CheckFaces(data, first, second, facesA, true))
        return nullptr;

    if (this->CheckFaces(data, first, second, facesB, false))
        return nullptr;

    if (!data.isFaceCollision)
    {
        // TODO : Refactor this part as the same chunk is present in ShortestDistanceBetweenEdges
        std::pair<int, int> e1 = edgesA[data.indexEdgeA];
        std::pair<int, int> e2 = edgesB[data.indexEdgeB];
        std::pair<glm::vec3, glm::vec3> edgeA = std::make_pair(pointsA[e1.first], pointsA[e1.second]);
        std::pair<glm::vec3, glm::vec3> edgeB = std::make_pair(pointsB[e2.first], pointsB[e2.second]);
        collisionPoints.push_back(this->GetContactBetweenEdges(edgeA, edgeB));

        // adjust the direction of the normal.
        float direction = glm::dot(second->center - first->center, data.collisionAxis);
        if (direction > 0.f)
            data.collisionAxis = -data.collisionAxis;
    }
    else
    {
        if (data.isFaceACollision)
        {
            collisionPoints = this->GetCollisionPoints(data, pointsB, edgesB, facesA);
            data.collisionAxis = -data.collisionAxis;
        }
        else
        {
            collisionPoints = this->GetCollisionPoints(data, pointsA, edgesA, facesB);
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

bool CollisionDetector::IsSeparatingAxis(glm::vec3 direction,
                                        const std::vector<glm::vec3>& pointsA,
                                        const std::vector<glm::vec3>& pointsB,
                                        float& tempPenDepth)
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

glm::vec3 CollisionDetector::GetContactBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA,
                                                    const std::pair<glm::vec3, glm::vec3>& edgeB)
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

float CollisionDetector::GetMinDistanceBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, 
                                                    const std::pair<glm::vec3, glm::vec3>& edgeB)
{
    glm::vec3 e1 = edgeA.second - edgeA.first;
    glm::vec3 e2 = edgeB.second - edgeB.first;
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

std::vector<glm::vec3> CollisionDetector::Clip( const std::vector<glm::vec3>& points,
                                                const std::vector<std::pair<int, int>>& edges, 
                                                const std::vector<ColliderFace>& planes)
{
    std::vector<glm::vec3> output;
    for (int k = 0; k < edges.size(); k++)
    {
        glm::vec3 v1 = points[edges[k].first];
        glm::vec3 v2 = points[edges[k].second];

        bool shouldAddFirst = !this->ContainsPoint(v1, output);
        bool shouldAddSecond = !this->ContainsPoint(v2, output);

        if (!shouldAddFirst && !shouldAddSecond)
        {
            continue;
        }

        glm::vec3 intersectionPoint = glm::vec3(0.f, 0.f, 0.f);
        for (int j = 0; j < planes.size(); j++)
        {
            ColliderFace plane = planes[j];
            float dotFirst = glm::dot(plane.normal, v1);
            float dotSecond = glm::dot(plane.normal, v2);
            float planeOffset = glm::dot(plane.normal, points[plane.points[0]]);
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
                std::pair<glm::vec3, glm::vec3> planePair = std::make_pair(plane.normal, points[plane.points[0]]);
                glm::vec3 intersectionPoint = this->IntersectLinePlane(v1, v2, planePair);
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

glm::vec3 CollisionDetector::IntersectLinePlane(glm::vec3& a, glm::vec3& b, std::pair<glm::vec3, glm::vec3>& plane)
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

bool CollisionDetector::CheckFaces(SATData& data, 
                                    std::shared_ptr<Collider> first, 
                                    std::shared_ptr<Collider> second, 
                                    const std::vector<ColliderFace>& faces,
                                    bool isFaceA)
{

    const std::vector<glm::vec3>& pointsA = first->GetPoints();
    const std::vector<glm::vec3>& pointsB = second->GetPoints();
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
        if (this->IsSeparatingAxis(faces[i].normal, pointsA, pointsB, currPenDepth))
        {
            return true;
        }
        float sameDirAsCenters = glm::dot(centerDir, faces[i].normal);
        if (currPenDepth <= data.minPenDepth && sameDirAsCenters >= 0.f)
        {
            data.indexFace = i;
            data.isFaceACollision = isFaceA;
            data.isFaceCollision = true;
            data.collisionAxis = faces[i].normal;
            data.minPenDepth = currPenDepth;
        }
    }
    return false;
}

bool CollisionDetector::CheckEdges(SATData& data, std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{

    const std::vector<glm::vec3>&           pointsA = first->GetPoints();
    const std::vector<std::pair<int, int>>& edgesA  = first->GetEdges();

    const std::vector<glm::vec3>&           pointsB = second->GetPoints();
    const std::vector<std::pair<int, int>>& edgesB  = first->GetEdges();

    float currPenDepth = 0.f;
    for (int i = 0; i < edgesA.size(); i++)
    {
        for (int j = 0; j < edgesB.size(); j++)
        {
            // take the cross
            glm::vec3 edgeA = pointsA[edgesA[i].second] - pointsA[edgesA[i].first];
            glm::vec3 edgeB = pointsB[edgesB[j].second] - pointsB[edgesB[j].first];
            glm::vec3 possibleCollisionAxis = glm::normalize(glm::cross(edgeA, edgeB));

            if (glm::length2(possibleCollisionAxis) < 0.005f)
                continue;

            // SAT check
            if (this->IsSeparatingAxis(possibleCollisionAxis, pointsA, pointsB, currPenDepth))
            {
                return true;
            }
            // we need this as we can have multiple edges with the same penetration depth,
            // but where the actual distance between the edges is different. AABB one such collider
            std::pair<glm::vec3, glm::vec3> e1 = std::make_pair(pointsA[edgesA[i].first], pointsA[edgesA[i].second]);
            std::pair<glm::vec3, glm::vec3> e2 = std::make_pair(pointsB[edgesB[j].first], pointsB[edgesB[j].second]);
            float currMinDistance = this->GetMinDistanceBetweenEdges(e1, e2);
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
                                                            const std::vector<std::pair<int, int>>& edges,
                                                            const std::vector<ColliderFace>& faces)
{

    std::vector<glm::vec3> collisionPoints;
    // get all 4 planes to clip against
    // clip the remaining points against the plane of the AABB with the same normal as the axisFase
    std::vector<ColliderFace> planes;
    for (int i = 0; i < faces.size(); i++)
    {
        glm::vec3 normal = faces[i].normal;
        if (glm::dot(normal, data.collisionAxis) == 0.0f)
        {
            planes.push_back(faces[i]);
        }
    }
    std::vector<glm::vec3> clippedPoints = this->Clip(points, edges, planes);
    for (int i = 0; i < clippedPoints.size(); i++)
    {
        glm::vec3 pointOnFace = points[faces[data.indexFace].points[0]];
        float dotFaceOne = glm::dot(data.collisionAxis, clippedPoints[i] - pointOnFace);
        if ( dotFaceOne <= 0.f)
        {
            collisionPoints.push_back(clippedPoints[i]);
        }
    }
    return collisionPoints;
}