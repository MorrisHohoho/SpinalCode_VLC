//
// Created by Administrator on 2023/6/19.
//

#ifndef TEST_LOOKUP3HASH_H
#define TEST_LOOKUP3HASH_H

#include<stdint.h>
#include<stddef.h>

static uint32_t state[3];

uint32_t hash_func(const int s, const int m);
#endif //TEST_LOOKUP3HASH_H
