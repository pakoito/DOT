#ifndef AILINK_HEADER
#define AILINK_HEADER
#include "World.h"
    #ifdef AILINK_EXPORTS
    #define AILINK __declspec(dllexport)
    #else
    #define AILINK __declspec(dllimport) 
    #endif

    class runner
    {
    public:
            static AILINK int startup(World* currentworld);
            static AILINK int run(World* currentworld, int* k, int* i, bool* running);
    };

#endif