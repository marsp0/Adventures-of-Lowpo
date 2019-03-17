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

        bool CheckCollision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second);

        std::shared_ptr<Collision> AABBToAABB(std::shared_ptr<AABB> first, std::shared_ptr<AABB> second);
        bool AABBToTriangle(std::shared_ptr<AABB> box, std::shared_ptr<Triangle> triangle);

        std::shared_ptr<Collision> TriangleToTriangle(std::shared_ptr<Triangle> first, std::shared_ptr<Triangle> second);
        bool TriangleToAABB(std::shared_ptr<Triangle> triangle, std::shared_ptr<AABB> box);

        /* 
        FindDistance
        Uses GJK to return the shortest distance between two convex polyhedras.
        https://en.wikipedia.org/wiki/Gilbert%E2%80%93Johnson%E2%80%93Keerthi_distance_algorithm
         */
        bool FindDistance(std::vector<glm::vec3>& pointsA, std::vector<glm::vec3>& pointsB);
        /* 
        GetSupport Points returns the the farthest point in the given direction.
        It is an O(n) operation, but the assumption here is that we wont have massive polyhedras.
         */
        glm::vec3 GetSupportPoint(std::vector<glm::vec3>& points, glm::vec3 direction);

        bool DoSimplex(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, glm::vec3& direction, int& simplexSize);

    private:
};