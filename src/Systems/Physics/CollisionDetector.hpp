#pragma once

#include <memory>
#include <vector>

#include "Collision.hpp"

#include "Collider.hpp"
#include "AABB.hpp"

/**
Collision detector contains all the logic that checks if two colliders are intersecting.
 */
class CollisionDetector
{
    public:
        CollisionDetector();

        std::shared_ptr<Collision> CheckCollision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second);
        std::shared_ptr<Collision> AABBToAABB(std::shared_ptr<AABB> first, std::shared_ptr<AABB> second);
        std::shared_ptr<Collision> Collide(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second);

        // Helpers
        /** 
        IsSeparatingAxis checks if a given axis separates two objects.
        we use the the support function to get the farhest points in dir and -dir
        and compare them to the points from the second object for overlap.
         */
        bool IsSeparatingAxis(  const std::vector<glm::vec3>& pointsA, 
                                const std::vector<glm::vec3>& pointsB, 
                                glm::vec3 direction, 
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
        glm::vec3 IntersectLinePlane(   glm::vec3 a, 
                                        glm::vec3 b, 
                                        std::pair<glm::vec3, float> plane);

        /** 
        ProjectPointOntoPlane projects a given point onto the given plane.
         */
        glm::vec3 ProjectPointOntoPlane(glm::vec3 point, 
                                        std::pair<glm::vec3, float> plane);
        /** 
        Clips a given list of edges against a plane. Used in collision detection to determine the contact
        points that are sent to the solver.
         */
        std::vector<glm::vec3> Clip(const std::vector<glm::vec3> points, 
                                    std::vector<std::pair<glm::vec3, float>> planes);
        /** 
        GetSupport Points returns the the farthest point in the given direction.
        It is an O(n) operation, but the assumption here is that we wont have massive polyhedras.
         */
        glm::vec3 GetSupportPoint(  const std::vector<glm::vec3>& points, 
                                    glm::vec3 direction);

    private:

        float tolerance = 0.0005f;
};