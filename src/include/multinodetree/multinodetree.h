#ifndef MULTINODETREE_H
#define MULTINODETREE_H


#ifndef CHILD_NUMS
#define CHILD_NUMS 16   // 2^k
#endif

struct MultiTree
{
    int message_int;    //猜测原始数据
    int rng;            //RNG
    int spine_value;    
    int depth;          //节点深度
    int cost;
    struct MultiTree *parent;   
    struct MultiTree *child[CHILD_NUMS];     //使用动态数组
};

void BuildChild(struct MultiTree* parent,int *symbol,int k,int c);

void PruningTree(struct MultiTree* parent,int k,int B);

void SortingTree(struct MultiTree* parent,int k);

#endif