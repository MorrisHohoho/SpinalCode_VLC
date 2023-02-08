#ifndef ENCODER_H
#define ENCODER_H

typedef struct
{
    int spineValue;
    int c; // The lenth of output
} RNG;

/*
 * Message to bytes packets;
 * s : message;
 * k : Each message packet has k bits;
 * res : the result of bytes packets;
 */
void DiviedMessage(const char *s, int k, char *res);

/*
 * convert int symbols to byte flows;
 * temp : int symbols;
 * res : the result of byte flows;
 * c : symbol bits;
*/
void tempSymbol2Symbols(const int *temp, char *res,int lenth,int packet_len,int c);

void SpinalEncode(const char* message,char * symbols,int symbol_packet_len,int k,int c);
#endif
