#pragma once

class ResourceManager
{
    public:
        ResourceManager();

        void                                    InitWorld();
        std::pair<unsigned int,unsigned int>    SetupBuffers(float* data, int size, bool animated);
};