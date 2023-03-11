#include <string.h>
#include <stdint.h>
#include "decoder/decoder.h"
#include "multinodetree/multinodetree.h"

extern int k;
extern int c;
extern int B;



void SpinalDecode(const char *symbols, const int symbols_packet_len,char *decoded_message, int message_len)
{
    int symbols_integer_len = message_len*8/k;
    if(message_len*8%k!=0)
    symbols_integer_len++;
    int symbols_integer[symbols_integer_len];

    for (int i = 0; i < symbols_integer_len; i++)
        symbols_integer[i] = 0;
    Symbols2Int(symbols, symbols_integer, symbols_integer_len);

    // Create Root
    struct MultiTree root = {0, 0, 0, -1, 0, NULL, {NULL}};
    BuildChild(&root, symbols_integer);

    /******BUILDING PRUNING TREE*********/
    VECTOR_INIT(beam);
    beam.pfVectorAdd(&beam, &root);
    VECTOR_INIT(candidate_vec);
    for (int i = 1; i < symbols_integer_len ; i++)
    {
        // for T in beam
        struct Candidate dummyHead = {0, NULL, NULL};
        struct Candidate *candidate_pointer = &dummyHead;
        for (int j = 0; j < beam.pfVectorTotal(&beam); j++)
        {
            // for T' in subtrees of (T)
            struct MultiTree *tmp_root = beam.pfVectorGet(&beam, j);
            for (int z = 0; z < CHILD_NUMS; z++)
            {
                if (tmp_root->child[z]->child[0] != NULL)
                    break;

                // Expand T' from depth d-1 to depth d.
                BuildChild(tmp_root->child[z], symbols_integer);
                // Compute and store path_cost in expanded nodes.
                SortingTree(tmp_root->child[z]);
                int tmp_cost = tmp_root->child[z]->child[0]->cost;

                struct Candidate tmp_candidate = {tmp_cost, tmp_root->child[z]};
                add_candidates(candidate_pointer, &tmp_candidate);
                candidate_pointer = candidate_pointer->next;
                candidate_vec.pfVectorAdd(&candidate_vec, candidate_pointer);
            }
        }
        // get B lowest cost candidates, breaking ties arbitrarily
        sorting_candidates(&candidate_vec, 0, candidate_vec.pfVectorTotal(&candidate_vec)-1);
        for (int tmpB = 0; tmpB < B; tmpB++)
        {
            struct Candidate* tmpBeam = (struct Candidate*)candidate_vec.pfVectorGet(&candidate_vec,tmpB);
            beam.pfVectorAdd(&beam, tmpBeam->tree);
        }

        //Delete candidate_vec
        int vecTotal=candidate_vec.pfVectorTotal(&candidate_vec);
        for(int i=vecTotal-1;i>=0;i--)
        candidate_vec.pfVectorDelete(&candidate_vec,i);

        //Delete the tmp candidate
        candidate_pointer=dummyHead.next;
        for(int i=0;i<vecTotal;i++)
        {
            struct Candidate* tmpPointer = candidate_pointer;
            if(candidate_pointer->next!=NULL)
            candidate_pointer=candidate_pointer->next;
            free(tmpPointer);
        }
    }


    //Find the best node.
    int minCost=INT32_MAX;
    struct MultiTree* best_node = &root;
    for(int i=1;i<beam.pfVectorTotal(&beam);i++)
    {
        if((i-1)%B==0)
        minCost=INT32_MAX;       
        struct MultiTree* current_node = beam.pfVectorGet(&beam,i);
        for(int j =i+B;j<i+B+B;j++)
        {
            if(j<beam.pfVectorTotal(&beam))
            {
            struct MultiTree* tmp_node = beam.pfVectorGet(&beam,j);
            if(tmp_node->parent==current_node)
            {
                int tmp_cost = current_node->cost+tmp_node->cost;
                if(tmp_cost<minCost)
                {
                    best_node=current_node;
                    minCost=tmp_cost;
                    if(j>=beam.pfVectorTotal(&beam)-B)
                    {
                        best_node=tmp_node;
                    }
                }
            }
            }
        }
    }
    int tailCost = INT32_MAX;
    struct Multitree *tailNode ;
    for(int i=0;i<CHILD_NUMS;i++)
    {
        if(best_node->child[i]->cost<=tailCost)
        {
            tailNode=best_node->child[i];
            tailCost=best_node->child[i]->cost;
        }
    }
    best_node=tailNode;

    getDecodedMessage(best_node,decoded_message,message_len);
}

void getDecodedMessage(struct MultiTree *node, char *decoded_message, int len)
{
    int pointer = 0;
    int counter = 0;
    for (int i = len-1; i >=0; i--)
    {
        
        for (int j = 0; j < 8; j++)
        {
            int decoded_symbol = node->message_int;
            decoded_message[i] |= ((decoded_symbol & (1 <<pointer))) >> pointer == 1 ? (1 << j) : 0;
            ++pointer;
            if (pointer == k)
            {
                node = node->parent;
                pointer = 0;
            }
        }
    }
}


void add_candidates(struct Candidate *head, struct Candidate *NEX)
{
    head->next = (struct Candidate *)malloc(sizeof(struct Candidate));
    head->next->cost = NEX->cost;
    head->next->tree = NEX->tree;
    head->next->next = NULL;
}

void sorting_candidates(vector *candidates, int l, int r)
{

    int tmpl,tmpr=0;
    if (l >= r)
        return;
    int i = l - 1, j = r + 1;
    struct Candidate *left;
    struct Candidate *right;
    struct Candidate *mid = (struct Candidate *)candidates->pfVectorGet(candidates, (l + r) >> 1);
    int x = mid->cost;
    while (i < j)
    {
        do
        {
            i++;
            left = (struct Candidate *)candidates->pfVectorGet(candidates, i);
        }while (left->cost < x);

        do
        {
            j--;
            right = (struct Candidate *)candidates->pfVectorGet(candidates, j);
        }while (right->cost > x);

        if (i < j)
        {
            struct Candidate *tmp = left;
            
            candidates->pfVectorSet(candidates,i,right);
            candidates->pfVectorSet(candidates,j,tmp);
        }
    }
    sorting_candidates(candidates, l, j), sorting_candidates(candidates, j + 1, r);
}

void Symbols2Int(const char *symbols, uint32_t *res, int len)
{
    int pointer = 0;
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < c; j++)
        {
            res[i] |= ((symbols[count] & (1 << (8 - pointer - 1))) >> (8 - pointer - 1)) == 1 ? (1 << (c - j - 1)) : 0;
            ++pointer;
            if (pointer == 8)
            {
                ++count;
                pointer = 0;
            }
        }
    }
}