//
// Created by Administrator on 2023/6/20.
//

#include "RNG/RNG.h"


 int next(RNG *rng)
{
    if(rng->count==0)
    {
        //produce more values
        uint32_t  hash_output = hash_func(rng->seed,rng->i);
        rng->next_values[0]=(hash_output>>16)&0xFFFF;
        rng->next_values[1]=(hash_output)&0xFFFF;
        rng->i=(rng->i+3243335647)&((1<<32)-1);
        rng->count=2;
    }

    int val = rng->next_values[0];
    rng->next_values[0]=rng->next_values[1];
    rng->count--;

    return val;

}

int map_func(const uint16_t value)
{
    int mask=(1<<C)-1;
    int bits=value&mask;

    return bits;
}

void  set_rng(RNG *rng, uint32_t spine_value)
{
    rng->seed=spine_value;
    rng->next_values[0]=(spine_value>>16)&0xFFFF;
    rng->next_values[1]=(spine_value)&0xFFFF;
    rng->count=2;
    rng->i=3610617884;
}