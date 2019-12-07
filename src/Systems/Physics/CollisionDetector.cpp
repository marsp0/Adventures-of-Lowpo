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
    std::vector<glm::vec3> contactPoints;

    // get edges and faces
    const std::vector<glm::vec3>&           pointsA = first->GetPoints();
    const std::vector<std::pair<int, int>>& edgesA  = first->GetEdges();

    const std::vector<glm::vec3>&           pointsB = second->GetPoints();
    const std::vector<std::pair<int, int>>& edgesB  = second->GetEdges();

    SATData data;
    data.isFaceACollision = false;
    data.isFaceCollision = false;
    data.minPenDepth = 10000.f;
    data.minEdgeDistance = 10000.f;

    if (this->CheckFaces(data, first, second, true))
        return nullptr;

    if (this->CheckFaces(data, second, first, false))
        return nullptr;

    if (this->CheckEdges(data, first, second))
        return nullptr;

    if (!data.isFaceCollision)
    {
        // TODO : Refactor this part as the same chunk is present in ShortestDistanceBetweenEdges
        std::pair<int, int> e1 = edgesA[data.indexEdgeA];
        std::pair<int, int> e2 = edgesB[data.indexEdgeB];
        std::pair<glm::vec3, glm::vec3> edgeA = std::make_pair(pointsA[e1.first], pointsA[e1.second]);
        std::pair<glm::vec3, glm::vec3> edgeB = std::make_pair(pointsB[e2.first], pointsB[e2.second]);
        contactPoints.push_back(this->GetContactBetweenEdges(edgeA, edgeB));

        // adjust the direction of the normal.
        float direction = glm::dot(second->center - first->center, data.collisionAxis);
        if (direction > 0.f)
            data.collisionAxis = -data.collisionAxis;
    }
    else
    {
        if (data.isFaceACollision)
        {
            contactPoints = this->GetContactPoints(data, first, second);
            data.collisionAxis = -data.collisionAxis;
        }
        else
            contactPoints = this->GetContactPoints(data, second, first);
    }

    assert(contactPoints.size() != 0);

    std::vector<Contact> contacts;
    std::shared_ptr<Collision> collision = std::make_shared<Collision>(first->entityID, first, second->entityID, second, contacts);
    for (int i = 0; i < contactPoints.size(); i++)
    {
        glm::vec3 point = contactPoints[i];
        Contact contact = Contact(point, data.collisionAxis, data.minPenDepth);
        collision->contacts.push_back(contact);
    }
    return collision;
}

bool CollisionDetector::CheckFaces(SATData& data, std::shared_ptr<Collider> first, std::shared_ptr<Collider> second, bool isFaceA)
{

    const std::vector<glm::vec3>& pointsA = first->GetPoints();
    const std::vector<glm::vec3>& pointsB = second->GetPoints();
    const std::vector<ColliderFace>& faces = first->GetFaces();

    glm::vec3 centerDir = second->center - first->center;

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

    float faceEdgeTolerance = 0.005f;
    float currPenDepth = 0.f;
    for (int i = 0; i < edgesA.size(); i++)
    {
        for (int j = 0; j < edgesB.size(); j++)
        {
            // take the cross
            glm::vec3 edgeA = pointsA[edgesA[i].second] - pointsA[edgesA[i].first];
            glm::vec3 edgeB = pointsB[edgesB[j].second] - pointsB[edgesB[j].first];
            glm::vec3 possibleCollisionAxis = glm::normalize(glm::cross(edgeA, edgeB));
            bool isMin = false;

            if (glm::length2(possibleCollisionAxis) < 0.005f)
                continue;

            // SAT check
            if (this->IsSeparatingAxis(possibleCollisionAxis, pointsA, pointsB, currPenDepth))
            {
                return true;
            }

            std::pair<glm::vec3, glm::vec3> e1 = std::make_pair(pointsA[edgesA[i].first], pointsA[edgesA[i].second]);
            std::pair<glm::vec3, glm::vec3> e2 = std::make_pair(pointsB[edgesB[j].first], pointsB[edgesB[j].second]);
            float currMinDistance = this->GetMinDistanceBetweenEdges(e1, e2);
            // we want to favor face collisions over edge ones.
            if (!data.isFaceCollision)
            {
                if (currPenDepth <= data.minPenDepth && currMinDistance < data.minEdgeDistance)
                    isMin = true;
            }
            else
            {
                if (currPenDepth + faceEdgeTolerance < data.minPenDepth && data.isFaceCollision)
                    isMin = true;
            }
            if (isMin)
            {
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
    sc = (abs(sNominator) < this->tolerance ? 0.0 : sNominator / sDenominator);
    tc = (abs(tNominator) < this->tolerance ? 0.0 : tNominator / tDenominator);
    glm::vec3 result = w + sc * e1 - tc * e2;
    // dont need actual distance
    return glm::length2(result);
}

std::vector<glm::vec3> CollisionDetector::GetContactPoints(SATData& data, std::shared_ptr<Collider> first, std::shared_ptr<Collider> second)
{
    const std::vector<ColliderFace>& incidentFaces = second->GetFaces();
    const std::vector<glm::vec3>& incidentPoints = second->GetPoints();

    const std::vector<glm::vec3>& referencePoints = first->GetPoints();

    float min = 10000.f;
    int index = -1;
    for (int i = 0; i < incidentFaces.size(); i++)
    {
        float currentMin = glm::dot(data.collisionAxis, incidentFaces[i].normal);
        if (currentMin < min)
        {
            min = currentMin;
            index = i;
        }
    }

    // create points vector
    std::vector<glm::vec3> incidentFacePoints;
    for (int i = 0; i < incidentFaces[index].points.size(); i++)
    {
        int pointIndex = incidentFaces[index].points[i];
        incidentFacePoints.push_back(incidentPoints[pointIndex]);
    }

    // create faces to clip against
    std::vector<std::pair<glm::vec3, glm::vec3>> sideFaces;
    const ColliderFace& referenceFace = first->GetFaces()[data.indexFace];
    int count = referenceFace.points.size();
    for (int i = 0; i < referenceFace.points.size(); i++)
    {
        int v1 = referenceFace.points[i];
        int v2 = referenceFace.points[(i + 1) % count];
        glm::vec3 edge = referencePoints[v2] - referencePoints[v1];
        glm::vec3 normal = glm::normalize(glm::cross( referenceFace.normal, edge));
        if (glm::dot(referencePoints[v1] - first->center, normal) < 0.f)
            normal = -normal;
        sideFaces.push_back(std::make_pair(normal, referencePoints[v1]));
    }

    // clip
    std::vector<glm::vec3> clippedPoints = this->Clip(incidentFacePoints, sideFaces);
    std::vector<glm::vec3> contactPoints;
    for (int i = 0; i < clippedPoints.size(); i++)
    {
        if ( glm::dot(data.collisionAxis, clippedPoints[i] - sideFaces[0].second) <= 0.f)
            contactPoints.push_back(clippedPoints[i]);
    }
    return contactPoints;
}

std::vector<glm::vec3> CollisionDetector::Clip(std::vector<glm::vec3>& points, std::vector<std::pair<glm::vec3, glm::vec3>>& planes)
{
    // Used Sutherland-Hodgman for the clipping
    // https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
    std::vector<glm::vec3> output = points;
    for (int j = 0; j < planes.size(); j++)
    {
        std::vector<glm::vec3> input = output;
        output.clear();
        std::pair<glm::vec3, glm::vec3> plane = planes[j];
        float planeOffset = glm::dot(plane.first, plane.second);
        for (int i = 0; i < input.size(); i++)
        {
            // compute intersection points
            glm::vec3 v1 = input[i];
            glm::vec3 v2 = input[(i+1) % input.size()];
            glm::vec3 intersectionPoint = this->IntersectLinePlane(v1, v2, plane);
            if (glm::dot(plane.first, v2) < planeOffset)
            {
                if (glm::dot(plane.first, v1) > planeOffset)
                {
                    output.push_back(intersectionPoint);
                }
                output.push_back(v2);
            }
            else if (glm::dot(plane.first, v1) < planeOffset)
            {
                output.push_back(intersectionPoint);
            }
        }
    }
    return output;
}

glm::vec3 CollisionDetector::IntersectLinePlane(glm::vec3 a, glm::vec3 b, std::pair<glm::vec3, glm::vec3>& plane)
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