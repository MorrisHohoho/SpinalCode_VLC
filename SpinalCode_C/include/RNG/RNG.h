//
// Created by Administrator on 2023/6/20.
//

#ifndef TEST_RNG_H
#define TEST_RNG_H

#include "jenkins_hash/Lookup3Hash.h"
#include "constant.h"
#include<stdint.h>

typedef struct
{
    uint32_t seed;
    uint16_t next_values[2];
    uint32_t i;
    int count;
} RNG;

int next(RNG *rng);

int map_func(const uint16_t value);

void  set_rng(RNG *rng, uint32_t spine_value);

#endif //TEST_RNG_H
