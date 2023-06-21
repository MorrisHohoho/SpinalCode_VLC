//
// Created by Administrator on 2023/6/19.
//

#include "jenkins_hash/Lookup3Hash.h"
#include<memory.h>

static inline uint32_t rot(uint32_t x, int k)
{
    return (x << k) | (x >> (32 - k));
}

void look3upInit(const int val)
{
    state[0]=0xdeadbeef+val;
    state[1]=0xdeadbeef+val;
    state[2]=0xdeadbeef+val;
}

void lookup3Update(const int data)
{
    state[1] += data;
    state[2] ^= state[1]; state[2] -= rot(state[1],14);
    state[0] ^= state[2]; state[0] -= rot(state[2],11);
    state[1] ^= state[0]; state[1] -= rot(state[0],25);
    state[2] ^= state[1]; state[2] -= rot(state[1],16);
    state[0] ^= state[2]; state[0] -= rot(state[2],4);
    state[1] ^= state[0]; state[1] -= rot(state[0],14);
    state[2] ^= state[1]; state[2] -= rot(state[1],24);
}

uint32_t lookup3Digest()
{
    return state[2];
}

uint32_t hash_func(const int s, const int m)
{
    look3upInit(s);
    lookup3Update(m);
    return lookup3Digest();
}




