#ifndef ENCODER_H
#define ENCODER_H
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
typedef struct
{
    uint32_t spineValue; //hash state
    int c; //The lenth of symbol
} RNG;

/*
 * Divide Message into pieces,each piece has k bit;
 * s : message;
 * k : Each piece has k bits;
 * res : the result of message piece
 * 
 * e.g.: mes="1233",k=10;
 *   p1="00110001(1),00";
 *   p2="110010(2),0011";
 *   p3="0011(3),001100";
 *   p4="11(3),00000000";   
 */
void DiviedMessage(const char *s, int k, uint32_t *res);

/*
 * convert int symbols to byte flows;
 * temp : int symbols;
 * res : the result of byte flows;
 * c : symbol bits; 
*/
void tempSymbol2Symbols(const int *temp, char *res,int lenth,int packet_len,int c);

void SpinalEncode(const char* message,char * symbols,int symbol_packet_len,int k,int c);
#endif
