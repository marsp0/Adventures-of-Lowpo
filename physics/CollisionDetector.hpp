#pragma once

#include <memory>
#include <vector>

#include "Collision.hpp"

#include "AABB.hpp"
#include "Collider.hpp"
#include "Triangle.hpp"

/* 
Collision detector contains all the logic that checks if two colliders are intersecting.
 */
class CollisionDetector
{
    public:
        CollisionDetector();

        bool                        CheckCollision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second);
        
        bool                        AABBToTriangle(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle);
        std::shared_ptr<Collision>  AABBToAABB(std::shared_ptr<AABB> first, std::shared_ptr<AABB> second);
        bool                        TriangleToAABB(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box);
        std::shared_ptr<Collision>  TriangleToTriangle(std::shared_ptr<Triangle> first, std::shared_ptr<Triangle> second);
        
        bool                        IsSeparatingAxis(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB, glm::vec3 direction, float& tempPenDepth);
        float                       ShortestDistanceBetweenEdges(const std::pair<glm::vec3, glm::vec3>& edgeA, const std::pair<glm::vec3, glm::vec3>& edgeB);
        glm::vec3                   IntersectLinePlane(glm::vec3 a, glm::vec3 b, std::pair<glm::vec3, float> plane);
        glm::vec3                   ProjectPointOntoPlane(glm::vec3 point, std::pair<glm::vec3, float> plane);
        /* 
        Clips a given list of edges against a plane. Used in collision detection to determine the contact
        points that are sent to the solver.
         */
        std::vector<glm::vec3>      Clip(std::vector<glm::vec3> points, std::vector<std::pair<glm::vec3, float>> planes);
        /* 
        GetSupport Points returns the the farthest point in the given direction.
        It is an O(n) operation, but the assumption here is that we wont have massive polyhedras.
         */
        glm::vec3                   GetSupportPoint(std::vector<glm::vec3>& points, glm::vec3 direction);
    private:
};