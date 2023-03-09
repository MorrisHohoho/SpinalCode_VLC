#ifndef MULTINODETREE_H
#define MULTINODETREE_H

#ifndef CHILD_NUMS
#define CHILD_NUMS 16   // 2^k
#endif

struct MultiTree
{
    int message_int;    //speculated message
    int rng;            //RNG
    int spine_value;    
    int depth;          
    int cost;
    struct MultiTree *parent;   
    struct MultiTree *child[CHILD_NUMS];     
};


//Build child nodes and calculate cost.
void BuildChild(struct MultiTree* parent,int *symbol);


//Sorting child nodes in terms of cost, from lowest to highest.(Sorting algorithm is quick_sort)
void SortingTree(struct MultiTree* parent);

#endif