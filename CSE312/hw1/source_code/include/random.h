#ifndef __MYOS__SIMPLE_RANDOM
#define __MYOS__SIMPLE_RANDOM

#include <common/types.h>

namespace myos
{
        

    class SimpleRandom
    {
        
    private:
        unsigned int seed;

    public:
        SimpleRandom(unsigned int init_seed) : seed(init_seed) {}

        unsigned int next()
        {
            seed = 1664525 * seed + 1013904223;
            return seed;
        }

        unsigned int next_range(unsigned int min, unsigned int max)
        {
            return min + next() % (max - min);
        }
    };

}


#endif