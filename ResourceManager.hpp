#pragma once

#include "Loader.hpp"

class ResourceManager
{
    public:
        ResourceManager();

        void                                    InitWorld();
        std::vector<float>                      BuildBufferData(std::shared_ptr<Geometry> geometry);
        std::pair<unsigned int,unsigned int>    SetupBuffers(float* data, int size, bool animated);
};