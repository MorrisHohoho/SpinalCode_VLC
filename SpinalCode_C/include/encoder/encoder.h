#ifndef ENCODER_H
#define ENCODER_H

#include "RNG/RNG.h"
#include "jenkins_hash/Lookup3Hash.h"
#include "constant.h"

#include<stdint.h>
#include<string.h>
#include<stdlib.h>

/*
 * SpianlCode encode.
 * message : oringinal message;
 * @return: return transmitting symbols;
*/
uint8_t * SpinalEncode(const char* message);
#endif
