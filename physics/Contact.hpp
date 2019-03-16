#pragma once

#include <glm/glm.hpp>
/* 
Contact represents a single contact between 2 colliders.
 */
class Contact
{
    public:

        /* 
        Holds the point of contact in _______ space 
        */
        glm::vec3 contactPoint;
        /* 
        Contact normal 
        */
        glm::vec3 contactNormal;
        /*
        Penetration value 
        */
        float penetration;
    private:
};