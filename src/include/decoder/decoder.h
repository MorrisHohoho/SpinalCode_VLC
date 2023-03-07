#ifndef DECODER_H
#define DECODER_H
#define EXPAND_DEPTH 1
#include "multinodetree/multinodetree.h"
#include"vector/vector.h"
struct Candidate
{
    int cost;
    struct MultiTree *tree;
    struct Candidate *next;
};



void Symbols2Int(const  char *symbols, int *res,int len,int c);


void SpinalDecode(const char* symbols,char * decoded_message,int message_len,int k,int c,int B);

void sorting_candidates(vector* candidates,int l,int r);

void add_candidates(struct Candidate *head, struct Candidate *NEX);


#endif