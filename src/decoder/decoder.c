#include <string.h>
#include <stdint.h>
#include "decoder/decoder.h"
#include "multinodetree/multinodetree.h"

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
    PruningTree(&root, k, B);



    /***********WARNING:BUG HERE!*************/
    // 当出现多个一样的symbol的时候，解码会出错
    //Build Tree
    struct MultiTree *pointer = &root;
    for (int i = 1; i < symbols_integer_len; i++)
    {
        for (int j = 0; j < B; j++)
        {
                BuildChild(pointer->child[j], symbols_integer, k, c);
                PruningTree(pointer->child[j], k, B);
        }

        pointer = pointer->child[0]; // 指向下一层;
    }

    //Get Decode Symbol
    pointer = &root;
    int decoded_symbol[symbols_integer_len];
    for(int i=0;i<symbols_integer_len;i++)
    decoded_symbol[i]=0;
    getDecodedSymbols(pointer,decoded_symbol,symbols_integer_len,B);

    getDecodedMessage(decoded_symbol,decoded_message,message_len,k);


}