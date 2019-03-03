#pragma once

#include <glm/glm.hpp>
#include <memory>

enum ColliderType
{
    TRIANGLE,
    SPHERE,
    BOX
};

class Triangle;
class Sphere;
class AABB;

class Collider : public std::enable_shared_from_this<Collider>
{
    public:

        Collider(glm::vec3 center, bool dynamic, ColliderType colliderType);
        virtual ~Collider();
        virtual bool CheckCollision(std::shared_ptr<Collider> collider) = 0;
        virtual bool CheckCollision(std::shared_ptr<Triangle> triangle) = 0;
        virtual bool CheckCollision(std::shared_ptr<Sphere> sphere) = 0;
        virtual bool CheckCollision(std::shared_ptr<AABB> box) = 0;

        bool            dynamic;
        glm::vec3       center;
        ColliderType    type;
};