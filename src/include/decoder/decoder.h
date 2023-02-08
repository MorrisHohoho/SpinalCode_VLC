#ifndef DECODER_H
#define DECODER_H

void Symbols2Int(const  char *symbols, int *res,int len,int c);


void SpinalDecode(const char* symbols,char * decoded_message,int message_len,int k,int c,int B);

#endif