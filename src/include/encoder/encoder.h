#ifndef ENCODER_H
#define ENCODER_H
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

typedef struct
{
    uint32_t spineValue; //hash state
} RNG;

/*
 * Divide Message into pieces,each piece has k bit;
 * s : message;
 * res : the result of message piece
 * 
 * e.g.: mes="1233",k=10;
 *   p1="00110001(1),00";
 *   p2="110010(2),0011";
 *   p3="0011(3),001100";
 *   p4="11(3),00000000";   
 */
void diviedMessage(const char *s, uint32_t *res);

/*
 * convert int symbols to byte flows;
 * temp : int symbols;
 * res : the result of byte flows;
 * 
 * e.g. : c=6, tempOne = 001111  tempTwo = 001001
 * 
 * symbol[0] = [0,0,1,1,1,1,0,0];
 * symbol[1] = [1,0,0,1,X,X,X,X];
 * ...
*/
void tempSymbol2Symbols(const int *temp, char *res,int lenth,int packet_len);


/*
 * SpianlCode encode.
 * message : oringinal message;
 * symbols : result of encoding.
 * symbol_packet_len: the lenth of symbol.
*/
void SpinalEncode(const char* message,char * symbols,int symbol_packet_len);
#endif
