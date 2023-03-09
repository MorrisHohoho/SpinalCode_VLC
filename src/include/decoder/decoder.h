#ifndef DECODER_H
#define DECODER_H
#define EXPAND_DEPTH 1
#include<stdint.h>
#include "multinodetree/multinodetree.h"
#include"vector/vector.h"

struct Candidate
{
    int cost;
    struct MultiTree *tree;     //The address of tree node.
    struct Candidate *next;     //The address of next candidate.
};

//Add candidate node to candidates vector.
void add_candidates(struct Candidate *head, struct Candidate *NEX);


//Sorting candidates in terms of cost from lowest to highest.(Using quick_sort algorithm)
void sorting_candidates(vector* candidates,int l,int r);


//Recover symbols from seperated byte flow.
void Symbols2Int(const  char *symbols, uint32_t *res,int len);


//Recover message from seperated k bits.
void getDecodedMessage(struct MultiTree *node, char *decoded_message, int len);

/*
 * SpinalCode decode
 */
void SpinalDecode(const char* symbols,const int symbols_packet_len,char * decoded_message,int message_len);



#endif