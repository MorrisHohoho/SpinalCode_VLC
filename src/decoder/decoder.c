#include <string.h>
#include <stdint.h>
#include "decoder/decoder.h"
#include "multinodetree/multinodetree.h"
#include "vector/vector.h"

void Symbols2Int(const  char *symbols, int *res,int len,int c)
{
    int pointer = 0;
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < c; j++)
        {
            res[i] |= ((symbols[count] & (1 << (8 - pointer - 1))) >> (8 - pointer - 1)) == 1 ? (1 << (c- j - 1)) : 0;
            ++pointer;
            if (pointer == 8)
            {
                ++count;
                pointer = 0;
            }
        }
    }
}

void getDecodedSymbols(struct MultiTree *pointer ,int * decoded_symbol,int len,int B)
{

        int tmp_cost[B];
        for(int i=0;i<B;i++)
        tmp_cost[i]=0;
        for (int i = 0; i < len; i++)
    {
        // for (int j = 0; j < B; j++)
        // {
        //     if (pointer->child[j]->child[0] != NULL)
        //         tmp_cost[j] = pointer->child[j]->cost + pointer->child[j]->child[0]->cost;
        //     else
        //         tmp_cost[j] = pointer->child[j]->cost;
        // }
        // int tmp_caluate = INT16_MAX;
        // int index = 0;
        // for (int j = 0; j < B; j++)
        // {
        //     if (tmp_cost[j] < tmp_caluate)
        //     {
        //         tmp_caluate = tmp_cost[j];
        //         index = j;
        //     }
        // }
        decoded_symbol[i] = pointer->child[0]->message_int;
        pointer = pointer->child[0];
    }
    
}

void getDecodedMessage(const int* decoded_symbol,char * decoded_message,int len,int k)
{
    int pointer = 0;
    int counter = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            decoded_message[i] |= ((decoded_symbol[counter] & (1 << k - pointer - 1))) >> (k - pointer - 1) == 1 ? (1 << 8 - j - 1) : 0;
            ++pointer;
            if (pointer == k)
            {
                ++counter;
                pointer = 0;
            }
        }
        printf("%c", decoded_message[i]);
    }
}

void SpinalDecode(const char* symbols,char * decoded_message,int message_len,int k,int c,int B)
{
    int symbols_integer_len= strlen(symbols)*8/c;
    int symbols_integer[symbols_integer_len];

    for(int i=0;i<symbols_integer_len;i++)
    symbols_integer[i]=0;
    Symbols2Int(symbols,symbols_integer,symbols_integer_len,c);

    //Create Root
    struct MultiTree root = {0, 0, 0, -1, 0, NULL, {NULL}};
    BuildChild(&root, symbols_integer, k, c);
    //PruningTree(&root, k, B);


    //build pruning tree
    VECTOR_INIT(beam);
    beam.pfVectorAdd(&beam,&root);

    for(int i=1;i<symbols_integer_len-EXPAND_DEPTH;i++)
    {
        //for T in beam
        struct Candidate candidates[CHILD_NUMS];
        for(int j=0;j<beam.pfVectorTotal(&beam);j++)
        {
            //for T' in subtrees of (T)
            struct MultiTree* tmp_root = beam.pfVectorGet(&beam,j);
            for(int z=0;z<CHILD_NUMS;z++)
            {
                if(tmp_root->child[z]->child[0]!=NULL)
                break;
                
                //Expand T' from depth d-1 to depth d.
                BuildChild(tmp_root->child[z],symbols_integer,k,c);
                //Compute and store path_cost in expanded nodes.
                SortingTree(tmp_root->child[z],k);
                int tmp_cost =tmp_root->child[z]->cost+tmp_root->child[z]->child[0]->cost;

                /***********************BUG HERE*************************/
                //Should use data Structure like vector to contains candidates.
                struct Candidate tmp_candidate = {tmp_cost,tmp_root->child[z]};
                candidates[z]=tmp_candidate;
                //
            }
            //get B lowest cost candidates, breaking ties arbitrarily
            sorting_candidates(candidates,0,(1<<k)-1);
        }
        for(int tmpB=0;tmpB<B;tmpB++)
        {
                beam.pfVectorAdd(&beam,candidates[tmpB].tree);
        }
    }

    int XXXX=1;

    // struct MultiTree *pointer = &root;
    // for (int i = 1; i < symbols_integer_len; i++)
    // {
    //     for (int j = 0; j < B; j++)
    //     {
    //             BuildChild(pointer->child[j], symbols_integer, k, c);
    //             PruningTree(pointer->child[j], k, B);
    //     }

    //     pointer = pointer->child[0]; // 指向下一层;
    // }

    // //Get Decode Symbol
    // pointer = &root;
    // int decoded_symbol[symbols_integer_len];
    // for(int i=0;i<symbols_integer_len;i++)
    // decoded_symbol[i]=0;
    // getDecodedSymbols(pointer,decoded_symbol,symbols_integer_len,B);

    // getDecodedMessage(decoded_symbol,decoded_message,message_len,k);


}


void candidates_swap(struct Candidate * a,struct Candidate * b)
{
    struct Candidate c;
    c = *b ;
    *b = *a;
    *a = c;
}
void sorting_candidates(struct Candidate* candidates,int l,int r)
{
    if(l>=r) return;
    int i=l-1,j=r+1,x=candidates[l+r>>1].cost;
    while(i<j)
    {
        do i++;while(candidates[i].cost<x);
        do j--;while(candidates[j].cost>x);

        if(i<j) 
        {
            candidates_swap(&candidates[i],&candidates[j]);
        }
    }
    sorting_candidates(candidates,l,j),sorting_candidates(candidates,j+1,r);
}