#pragma once

#include <glm/glm.hpp>
#include <string>

class Joint
{
    public:
        Joint();
        ~Joint();

        glm::mat4   transform;
        std::string name;
        int         parentIndex;
        

};