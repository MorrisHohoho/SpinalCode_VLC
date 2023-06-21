#ifndef DECODER_H
#define DECODER_H


#include "RNG/RNG.h"
#include "jenkins_hash/Lookup3Hash.h"
#include "constant.h"
#include<stdint.h>

#define min(a,b) (((a)<(b))?(a):(b))


typedef struct
{
    int path_metric;
    uint32_t spine_value;
    uint8_t  path[SPINE_LENGTH];
}Wavefront;


/*
 * SpinalCode decode
 */
void SpinalDecode(const uint8_t * symbols, uint8_t * decoded_message);



#endif