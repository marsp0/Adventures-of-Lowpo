#pragma once

#include <memory>
#include <vector>

#include "Collision.hpp"

#include "Collider.hpp"

class SATData
{
    public:
        int         indexFace;
        int         indexEdgeA;
        int         indexEdgeB;
        bool        isFaceACollision;
        bool        isFaceCollision;
        float       minPenDepth;
        float       minEdgeDistance;
        glm::vec3   collisionAxis;
};

/**
Collision detector contains all the logic that checks if two colliders are intersecting.
 */
class CollisionDetector
{
    public:
        CollisionDetector();

        std::shared_ptr<Collision> Collide(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second);

        bool CheckFaces(SATData& data, std::shared_ptr<Collider> first,  std::shared_ptr<Collider> second,  const std::vector<ColliderFace>& faces, bool isFaceA);

        bool CheckEdges(SATData& data, std::shared_ptr<Collider> first, std::shared_ptr<Collider> second);

        std::vector<glm::vec3> GetCollisionPoints(SATData& data,
                                                  const std::vector<glm::vec3>& points,
                                                  const std::vector<std::pair<int, int>>& edges,
                                                  const std::vector<ColliderFace>& faces);

        // Helpers
        /** 
        IsSeparatingAxis checks if a given axis separates two objects.
        we use the the support function to get the farhest points in dir and -dir
        and compare them to the points from the second object for overlap.
         */
        bool IsSeparatingAxis(  glm::vec3 direction,
                                const std::vector<glm::vec3>& pointsA,
                                const std::vector<glm::vec3>& pointsB,
                                float& tempPenDepth);

        glm::vec3 GetContactBetweenEdges(   const std::pair<glm::vec3, glm::vec3>& edgeA,
                                            const std::pair<glm::vec3, glm::vec3>& edgeB);
        /**
        GetMinDistanceBetweenEdges - returns the shortest vector between two given edges.
         */
        float GetMinDistanceBetweenEdges(   const std::pair<glm::vec3, glm::vec3>& edgeA, 
                                            const std::pair<glm::vec3, glm::vec3>& edgeB);
        /** 
        IntersectLinePlane - checks if a given line intersects the given plane and if so returns
        the intersection point.
         */
        glm::vec3 IntersectLinePlane(glm::vec3& a, glm::vec3& b, std::pair<glm::vec3, glm::vec3>& plane);
        /** 
        Clips a given list of edges against a plane. Used in collision detection to determine the contact
        points that are sent to the solver.
         */
        std::vector<glm::vec3> Clip(const std::vector<glm::vec3>& points,
                                    const std::vector<std::pair<int, int>>& edges, 
                                    const std::vector<ColliderFace>& planes);

        bool ContainsPoint(glm::vec3& point, std::vector<glm::vec3>& points);
        /** 
        GetSupport Points returns the the farthest point in the given direction.
        It is an O(n) operation, but the assumption here is that we wont have massive polyhedras.
         */
        glm::vec3 GetSupportPoint(  const std::vector<glm::vec3>& points, 
                                    glm::vec3 direction);

        float tolerance = 0.0005f;
        
    private:

};