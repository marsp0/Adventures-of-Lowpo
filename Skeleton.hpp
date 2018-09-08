#pragma once

#include <vector>

#include "Joint.hpp"

class Skeleton
{
    public:

        Skeleton();
        ~Skeleton();

        int jointCounter;
        std::vector<Joint> joints;

};