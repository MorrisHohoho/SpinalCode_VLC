//
// Created by Administrator on 2023/6/21.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H

#define MES_LENGTH 4  //MAX_LENGTH is 8, because UINT64_MAX is 8 bytes

//Spinalcodes parameters
#define K 4 //BUG: if K >4, the program will crash
#define C 6  //MAX C is 8 because of the size of uint8_t
#define B 4 //BUG: if B >4, the program will crash
#define D 2 //BUG: if D >2, the program will crash
#define PASS 3

#define WAVEFRONT_MAX B*(1<<K)*(1<<K)
#define SUBTREES_MAX B*(1<<K)
#define SPINE_LENGTH ((8*MES_LENGTH+K-1)/K)

#endif //TEST_CONSTANT_H
