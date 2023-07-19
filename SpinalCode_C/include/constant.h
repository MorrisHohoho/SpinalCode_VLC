//
// Created by Administrator on 2023/6/21.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H

#define MES_LENGTH 4  //MAX_LENGTH is 8, because UINT64_MAX is 8 bytes

//Spinalcodes parameters
#define K 3 // no more than 8
#define C 5  //MAX C is 8 because of the size of uint8_t
#define B 4
#define D 2 // if D > 5, the decoding latency is up to 12s.
#define PASS 3

#if (D==1)
#define WAVEFRONT_MAX B*(1<<(K*(D)))*(1<<(K*(D)))
#define SUBTREES_MAX B*(1<<(K*(D)))
#else
#define WAVEFRONT_MAX B*(1<<(K*(D-1)))*(1<<(K*(D-1)))
#define SUBTREES_MAX B*(1<<(K*(D-1)))
#endif

#define SPINE_LENGTH ((8*MES_LENGTH+K-1)/K)
#define PASS_LENGTH (SPINE_LENGTH*PASS)

#endif //TEST_CONSTANT_H
